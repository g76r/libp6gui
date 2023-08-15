/* Copyright 2017-2023 Hallowyn and others.
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
#include "graphicsitemutils.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

void GraphicsItemUtils::paintSelectionMarkIfSelected(
    QGraphicsItem *item, QPainter *painter,
    const QStyleOptionGraphicsItem *option) {
  // mostly inspired from qt_graphicsItem_highlightSelected
  // LATER use shape() instead of boundingRect()
  if (!item->isSelected())
    return;
  const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
  if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
    return;
  const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
  if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
    return;
  qreal itemPenWidth = 1.0;
  const qreal pad = itemPenWidth / 2;
  const qreal penWidth = 0; // cosmetic pen
  const QColor fgcolor = option->palette.windowText().color();
  const QColor bgcolor = option->palette.base().color();
  painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));
  painter->setPen(QPen(fgcolor, penWidth, Qt::DashLine));
  painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));
}
