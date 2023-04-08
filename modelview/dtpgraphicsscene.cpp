/* Copyright 2014-2023 Hallowyn and others.
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
#include "dtpgraphicsscene.h"
#include "dtp/perspectivewidget.h"
#include <QGraphicsView>
#include "dtp/targetmanager.h"
#include "dtpgraphicsview.h"
#include <QGraphicsItem>
#include "modelview/shareduiitem.h"
#include <QMultiHash>
#include <QtDebug>

DtpGraphicsScene::DtpGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  connect(this, &DtpGraphicsScene::selectionChanged,
          this, &DtpGraphicsScene::propagateSelectionChanged);
}

void DtpGraphicsScene::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DtpGraphicsScene::propagateSelectionChanged() {
  QByteArrayList ids;
  foreach(QGraphicsItem *i, selectedItems()) {
    auto qualifiedId = i->data(SharedUiItem::QualifiedIdRole)
        .toString().toUtf8();
    if (!qualifiedId.isEmpty()) {
      ids.append(qualifiedId);
    }
  }
  _selectedItemsIds = ids;
  emit selectedItemsChanged(ids);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        tm->setTarget(_perspectiveWidget, ids);
        DtpGraphicsView *dgv = qobject_cast<DtpGraphicsView*>(gv);
        if (dgv)
          emit dgv->selectedItemsChanged(ids);
        break;
      }
  }
}

void DtpGraphicsScene::registerDtpGraphicsItem(DtpGraphicsItem *graphicsItem,
                                               SharedUiItemList<> uiItems) {
  bool first = true;
  for (const SharedUiItem &sui : uiItems) {
    if (first) {
      _itemsByMainUiItem.insert(sui.qualifiedId(), graphicsItem);
      first = false;
    }
    _registeredItems.insert(sui.qualifiedId(), graphicsItem);
  }
}

void DtpGraphicsScene::itemChanged(
    SharedUiItem newItem, SharedUiItem oldItem, QByteArray idQualifier) {
  if (!_itemQualifierFilter.isEmpty()
      && !_itemQualifierFilter.contains(idQualifier))
    return;
  _registeredItems.remove(oldItem.qualifiedId(), nullptr);
  if (_itemsByMainUiItem.value(oldItem.qualifiedId()).isNull())
    _itemsByMainUiItem.remove(oldItem.qualifiedId());
  auto items = _registeredItems.values(oldItem.qualifiedId());
  if (newItem.id() != oldItem.id()) {
    if (!oldItem.isNull()) {
      _registeredItems.remove(oldItem.qualifiedId());
      QPointer<DtpGraphicsItem> mainItem =
          _itemsByMainUiItem.take(oldItem.qualifiedId());
      if (!newItem.isNull()) {
        for (QPointer<DtpGraphicsItem> p : items)
          _registeredItems.insert(newItem.qualifiedId(), p);
        if (mainItem)
          _itemsByMainUiItem.insert(newItem.qualifiedId(), mainItem);
      }
    }
  }
  for (DtpGraphicsItem *dgi : items) {
    Q_ASSERT(dgi); // should always be true since 0 have been removed before
    dgi->itemChanged(newItem, oldItem, idQualifier);
  }
}

/*void DtpGraphicsScene::setMouseOverItem(QByteArrayList ids) {
  _mouseoverItemsIds = ids;
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, ids);
        break;
      }
  }
}*/
