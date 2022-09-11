/* Copyright 2015-2022 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dtpmatrixview.h"
#include "dtp/dtpdocumentmanager.h"
#include "modelview/shareduiitem.h"
#include <QtDebug>
#include "modelview/shareduiitemsmodel.h"
#include "modelview/shareduiitemsmatrixmodel.h"

// TODO factorize code with DtpTreeView

DtpMatrixView::DtpMatrixView(QWidget *parent)
  : EnhancedTableView(parent), _perspectiveWidget(0) {
  connect(this, &QAbstractItemView::entered,
          this, &DtpMatrixView::itemHovered);
  connect(this, &DtpMatrixView::viewportEntered,
          this, &DtpMatrixView::setEmptyMouseoverTarget);
  connect(this, &EnhancedTableView::leaved,
          this, &DtpMatrixView::clearMouseoverTarget);
}

void DtpMatrixView::setPerspectiveWidget(PerspectiveWidget *widget) {
  // TODO replace with setting by constructor, requiring parent is a PW
  _perspectiveWidget = widget;
}

void DtpMatrixView::setModel(QAbstractItemModel *newModel) {
  SharedUiItemsModel *m = _proxyModelHelper.realModel();
  if (m)
    disconnect(m, 0, this, 0);
  EnhancedTableView::setModel(newModel);
  _proxyModelHelper.setApparentModel(newModel);
  m = _proxyModelHelper.realModel();
  if (m) {
    connect(m, &SharedUiItemsModel::itemChanged,
            this, &DtpMatrixView::itemChanged);
  }
  auto *mm = qobject_cast<SharedUiItemsMatrixModel*>(m);
  if (mm) {
    connect(mm, &SharedUiItemsMatrixModel::headerBinded,
            this, &DtpMatrixView::headerBinded);
    connect(mm, &SharedUiItemsMatrixModel::cellBinded,
            this, &DtpMatrixView::cellBinded);
  }
  setMouseTracking(newModel);
  if (underMouse())
    setEmptyMouseoverTarget();
}

void DtpMatrixView::itemHovered(const QModelIndex &index) {
  QAbstractItemModel *m = model();
  QString id;
  // FIXME qDebug() << "DtpMatrixView::itemHovered" << index << m
  //<< (m?m->data(index, SharedUiItem::QualifiedIdRole):QVariant());
  if (m) {
    _mousePosition = index;
    id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
  }
  setMouseoverTarget(id);
}

void DtpMatrixView::setMouseoverTarget(QString itemId)  {
  if (itemId.isNull())
    _mousePosition = QPersistentModelIndex();
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    QStringList itemIds;
    if (!itemId.isNull())
      itemIds.append(itemId);
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, itemIds);
  }
}

void DtpMatrixView::clearMouseoverTarget() {
  _mousePosition = QPersistentModelIndex();
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}

void DtpMatrixView::selectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected) {
  EnhancedTableView::selectionChanged(selected, deselected);
  QAbstractItemModel *m = model();
  // LATER use a better way to identify rows than testing column 0, since this only works with selectionBehavior() == SelectRows
  if (m) {
    foreach(const QModelIndex &index, deselected.indexes()) {
      _selectedItemsIds.removeAll(index.data(SharedUiItem::QualifiedIdRole)
                                  .toString());
    }
    foreach(const QModelIndex &index, selected.indexes()) {
      QString id = index.data(SharedUiItem::QualifiedIdRole).toString();
      if (!_selectedItemsIds.contains(id))
        _selectedItemsIds.append(id);
    }
  }
  //qDebug() << "DtpMatrixView::selectionChanged" << hasFocus() << _selectedItemsIds;
  emit selectedItemsChanged(_selectedItemsIds);
  if (hasFocus()) {
    TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
    if (tm)
      tm->setTarget(_perspectiveWidget, _selectedItemsIds);
  }
}

void DtpMatrixView::itemChanged(SharedUiItem newItem, SharedUiItem oldItem) {
  // Update current selection when an item id changes.
  if (!oldItem.isNull()) { // new items cannot already be targeted
    QString newId = newItem.qualifiedId(), oldId = oldItem.qualifiedId();
    if (oldId != newId) { // only handle events where id changed
      if (newItem.isNull()) { // item removed
        _selectedItemsIds.removeAll(oldId);
      } else { // item renamed
        for (int i = 0; i < _selectedItemsIds.size(); ++i) {
          QString &id = _selectedItemsIds[i];
          if (id == oldId) {
            id = newId;
          }
        }
      }
      emit selectedItemsChanged(_selectedItemsIds);
    }
  }
  // ensure new or modified item is visible
  if (!newItem.isNull()) {
    QModelIndex index = _proxyModelHelper.indexOf(newItem);
    if (index.isValid())
      scrollTo(index);
  }
}

void DtpMatrixView::headerBinded(int section, Qt::Orientation orientation,
                  SharedUiItem newItem, SharedUiItem oldItem,
                  QString newFormula, QString newTooltip) {
  Q_UNUSED(section)
  Q_UNUSED(orientation)
  Q_UNUSED(newFormula)
  Q_UNUSED(newTooltip)
  itemChanged(newItem, oldItem);
}

void DtpMatrixView::cellBinded(int row, int column, SharedUiItem newItem,
                SharedUiItem oldItem, QString newFormula, QString newTooltip,
                int newEditableSection) {
  Q_UNUSED(row)
  Q_UNUSED(column)
  Q_UNUSED(newFormula)
  Q_UNUSED(newTooltip)
  Q_UNUSED(newEditableSection)
  itemChanged(newItem, oldItem);
}

QStringList DtpMatrixView::mouseoverItemsIds() const {
  QAbstractItemModel *m = model();
  return _mousePosition.isValid() && m
      ? QStringList(_mousePosition.data(SharedUiItem::QualifiedIdRole)
                    .toString())
      : QStringList();
}

bool DtpMatrixView::startItemEdition(QString qualifiedId) {
  SharedUiItemsProxyModelHelper helper(model());
  SharedUiItemsModel *m = helper.realModel();
  if (!m)
    return false;
  QModelIndex index = helper.indexOf(qualifiedId);
  if (!index.isValid())
    return false;
  scrollTo(index);
  clearSelection();
  setCurrentIndex(index);
  // LATER should test model & view are r/w before ?
  edit(index);
  return true;
}

void DtpMatrixView::commitData(QWidget *editor) {
  EnhancedTableView::commitData(editor);
  // when edition ends, data under mouse can have change because of sorting
  // and in this case no itemHovered() is not called, therefore mouseover
  // target is not updated
  clearMouseoverTarget(); // LATER item under mouse rather than no item
}
