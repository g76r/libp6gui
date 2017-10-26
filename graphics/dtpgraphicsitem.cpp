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

DtpGraphicsItem::DtpGraphicsItem(QGraphicsItem *parent)
  : QGraphicsObject(parent), _documentManager(0) {
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

void DtpGraphicsItem::setDocumentManager(DtpDocumentManager *documentManager) {
  _documentManager = documentManager;
  update();
}

void DtpGraphicsItem::persistPosition() {
  if (!_documentManager || !_uiItem)
    return;
  int section = _positionSection;
  if (section < 0)
    section = _uiItem.uiSectionByName(_positionSectionName);
  if (section >= 0)
    _documentManager->changeItemByUiData(_uiItem, section, pos());
   // TODO merge move transactions
}
