/* Copyright 2015-2023 Hallowyn and others.
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
#include "dtptableview.h"
#include "dtp/dtpdocumentmanager.h"

// TODO factorize code with DtpTreeView

DtpTableView::DtpTableView(QWidget *parent)
  : EnhancedTableView(parent), _perspectiveWidget(0) {
  connect(this, &QAbstractItemView::entered,
          this, &DtpTableView::itemHovered);
  connect(this, &DtpTableView::viewportEntered,
          this, &DtpTableView::setEmptyMouseoverTarget);
  connect(this, &EnhancedTableView::leaved,
          this, &DtpTableView::clearMouseoverTarget);
}

void DtpTableView::setPerspectiveWidget(PerspectiveWidget *widget) {
  // TODO replace with setting by constructor, requiring parent is a PW
  _perspectiveWidget = widget;
}

void DtpTableView::setModel(QAbstractItemModel *newModel) {
  SharedUiItemsModel *m = _proxyModelHelper.realModel();
  if (m) {
    disconnect(m, &SharedUiItemsModel::itemChanged,
               this, &DtpTableView::itemChanged);
  }
  EnhancedTableView::setModel(newModel);
  _proxyModelHelper.setApparentModel(newModel);
  m = _proxyModelHelper.realModel();
  if (m) {
    connect(m, &SharedUiItemsModel::itemChanged,
            this, &DtpTableView::itemChanged);
  }
  setMouseTracking(newModel);
  if (underMouse())
    setEmptyMouseoverTarget();
}

void DtpTableView::itemHovered(const QModelIndex &index) {
  QAbstractItemModel *m = model();
  QByteArray id;
  // FIXME qDebug() << "DtpTableView::itemHovered" << index << m
  //<< (m?m->data(index, SharedUiItem::QualifiedIdRole):QVariant());
  if (m) {
    _mousePosition = index;
    id = m->data(index, SharedUiItem::QualifiedIdRole).toString().toUtf8();
  }
  setMouseoverTarget(id);
}

void DtpTableView::setMouseoverTarget(QByteArray itemId)  {
  if (itemId.isNull())
    _mousePosition = QPersistentModelIndex();
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    QByteArrayList itemIds;
    if (!itemId.isNull())
      itemIds.append(itemId);
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, itemIds);
  }
}

void DtpTableView::clearMouseoverTarget() {
  _mousePosition = QPersistentModelIndex();
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}

void DtpTableView::selectionChanged(const QItemSelection &selected,
                                    const QItemSelection &deselected) {
  EnhancedTableView::selectionChanged(selected, deselected);
  QAbstractItemModel *m = model();
  // LATER use a better way to identify rows than testing column 0, since this only works with selectionBehavior() == SelectRows
  if (m) {
    for (const QModelIndex &index: deselected.indexes()) {
      if (index.column() == 0) {
        _selectedItemsIds.removeAll(index.data(SharedUiItem::QualifiedIdRole)
                                    .toString());
      }
    }
    for (const QModelIndex &index: selected.indexes()) {
      auto id = index.data(SharedUiItem::QualifiedIdRole).toString().toUtf8();
      if (index.column() == 0 && !_selectedItemsIds.contains(id)) {
        _selectedItemsIds.append(id);
      }
    }
  }
  //qDebug() << "DtpTableView::selectionChanged" << selected << deselected
  //        << _selectedItemsIds;
  emit selectedItemsChanged(_selectedItemsIds);
  if (hasFocus()) {
    TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
    if (tm)
      tm->setTarget(_perspectiveWidget, _selectedItemsIds);
  }
}

void DtpTableView::itemChanged(SharedUiItem newItem, SharedUiItem oldItem) {
  // Update current selection when an item id changes.
  if (!oldItem.isNull()) { // new items cannot already be targeted
    auto newId = newItem.qualifiedId(), oldId = oldItem.qualifiedId();
    if (oldId != newId) { // only handle events where id changed
      if (newItem.isNull()) { // item removed
        _selectedItemsIds.removeAll(oldId);
      } else { // item renamed
        for (int i = 0; i < _selectedItemsIds.size(); ++i) {
          QByteArray &id = _selectedItemsIds[i];
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

QByteArrayList DtpTableView::mouseoverItemsIds() const {
  QAbstractItemModel *m = model();
  return QByteArrayList{
    _mousePosition.isValid() && m
      ? _mousePosition.data(SharedUiItem::QualifiedIdRole).toString().toUtf8()
      : QByteArray{}
  };
}

bool DtpTableView::startItemEdition(QByteArray qualifiedId) {
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

void DtpTableView::commitData(QWidget *editor) {
  EnhancedTableView::commitData(editor);
  // when edition ends, data under mouse can have change because of sorting
  // and in this case no itemHovered() is not called, therefore mouseover
  // target is not updated
  clearMouseoverTarget(); // LATER item under mouse rather than no item
}
