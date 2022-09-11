/* Copyright 2017-2022 Hallowyn and others.
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
#include "dtpgraphicsitem.h"
#include "modelview/dtpgraphicsscene.h"
#include <QtDebug>
#include "modelview/dtpgraphicsscene.h"

DtpGraphicsItem::DtpGraphicsItem(QGraphicsItem *parent)
  : QGraphicsObject(parent) {
  connect(this, &DtpGraphicsItem::xChanged,
          this, &DtpGraphicsItem::persistPosition);
  connect(this, &DtpGraphicsItem::yChanged,
          this, &DtpGraphicsItem::persistPosition);
}

void DtpGraphicsItem::setUiItems(SharedUiItemList<> uiItems) {
  // LATER support being called twice or more: unregister, etc.
  //qDebug() << "DtpGraphicsItem::setUiItems" << this << uiItems.join(',', true);
  _uiItems = uiItems;
  if (uiItems.size())
    setData(SharedUiItem::QualifiedIdRole, uiItems.first().qualifiedId());
  //  qDebug() << "setting initial position" << uiData("position")
  //           << uiData("position").toPointF() << uiItem.uiData(4)
  //           << uiItem.uiData(4).toPointF() << uiItem.uiData(4).toString();
  QPointF pos = uiData(_positionSectionName).toPointF();
  if (!pos.isNull())
    setPos(pos);
  auto dtpScene = qobject_cast<DtpGraphicsScene*>(scene());
  if (dtpScene) {
    dtpScene->registerDtpGraphicsItem(this, _uiItems);
  } else {
    qWarning() << "DtpGraphicsItem::setUiItems() called before the item is "
                  "added to a DtpGraphicsScene, this will disable automatic "
                  "update of its SharedUiItems";
  }
  emit uiItemsChanged();
  update();
}

void DtpGraphicsItem::itemChanged(SharedUiItem newItem, SharedUiItem oldItem,
                                  QString idQualifier) {
  Q_UNUSED(idQualifier)
  int i = 0;
  for (SharedUiItem &item : _uiItems) {
    if (item.qualifiedId() == oldItem.qualifiedId()) {
      //qDebug() << "DtpGraphicsItem::itemChanged" << this << i
      //         << newItem.qualifiedId() << oldItem.qualifiedId()
      //         << idQualifier;
      if (newItem.isNull()) {
        auto graphicsScene = scene();
        if (graphicsScene && (_uiItems.isEmpty() || _uiItems.first() == item)) {
          //qDebug() << "  main ui item deleted -> self destructing";
          graphicsScene->removeItem(this);
          deleteLater();
        }
        _uiItems.removeAll(item); // safe because we break the loop just after
      } else {
        item = newItem;
        if (!i)
          setData(SharedUiItem::QualifiedIdRole, newItem.qualifiedId());
        emit uiItemsChanged();
        update();
      }
      break;
    }
    ++i;
  }
}

DtpDocumentManager *DtpGraphicsItem::documentManager() const {
  auto dtpScene = qobject_cast<DtpGraphicsScene*>(scene());
  auto pw = dtpScene ? dtpScene->perspectiveWidget() : 0;
  return pw ? pw->documentManager() : 0;
}

void DtpGraphicsItem::persistPosition() {
  auto dm = documentManager();
  if (!dm || _uiItems.isEmpty())
    return;
  SharedUiItem &sui = _uiItems[0];
  int section = sui.uiSectionByName(_positionSectionName);
  if (section < 0)
    return;
  QPointF oldPos = uiData(section).toPointF();
  QPointF newPos = pos();
  if (newPos == oldPos)
    return;
  //qDebug() << "persistPosition" << sui.qualifiedId() << section << oldPos << newPos;
  dm->changeItemByUiData(sui, section, pos());
  // TODO merge move transactions
}

QVariant DtpGraphicsItem::uiData(const QString &section, int role) const {
  QVariant v;
  for (const SharedUiItem &item : _uiItems) {
    v = item.uiDataBySectionName(section, role);
    if (!v.isNull())
      break;
  }
  return v;
}

QVariant DtpGraphicsItem::uiData(int section, int role) const {
  QVariant v;
  for (const SharedUiItem &item : _uiItems) {
    v = item.uiData(section, role);
    if (!v.isNull())
      break;
  }
  return v;
}
