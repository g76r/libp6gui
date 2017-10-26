/* Copyright 2017 Hallowyn and others.
 * This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
 * Libh6ncsu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libh6ncsu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dtpgraphicsitem.h"
#include "modelview/dtpgraphicsscene.h"
//#include <QtDebug>

DtpGraphicsItem::DtpGraphicsItem(QGraphicsItem *parent)
  : QGraphicsObject(parent) {
  connect(this, &DtpGraphicsItem::xChanged,
          this, &DtpGraphicsItem::persistPosition);
  connect(this, &DtpGraphicsItem::yChanged,
          this, &DtpGraphicsItem::persistPosition);
}

void DtpGraphicsItem::setUiItem(SharedUiItem uiItem) {
  _uiItem = uiItem;
  setData(SharedUiItem::QualifiedIdRole, _uiItem.qualifiedId());
  //  qDebug() << "setting initial position" << uiData("position")
  //           << uiData("position").toPointF() << uiItem.uiData(4)
  //           << uiItem.uiData(4).toPointF() << uiItem.uiData(4).toString();
  int section = _positionSection;
  if (section < 0)
    section = _uiItem.uiSectionByName(_positionSectionName);
  if (section >= 0)
    setPos(_uiItem.uiData(section).toPointF());
  update();
}

DtpDocumentManager *DtpGraphicsItem::documentManager() const {
  auto s = qobject_cast<DtpGraphicsScene*>(scene());
  auto pw = s ? s->perspectiveWidget() : 0;
  return  pw ? pw->documentManager() : 0;
}

void DtpGraphicsItem::persistPosition() {
  auto dm = documentManager();
  if (!dm || !_uiItem)
    return;
  int section = _positionSection;
  if (section < 0)
    section = _uiItem.uiSectionByName(_positionSectionName);
  if (section >= 0) {
    QPointF oldPos = _uiItem.uiData(section).toPointF();
    QPointF newPos = pos();
    //qDebug() << "persistPosition" << section << oldPos << newPos;
    if (newPos != oldPos)
      dm->changeItemByUiData(_uiItem, section, pos());
  }
   // TODO merge move transactions
}
