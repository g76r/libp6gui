/* Copyright 2014-2015 Hallowyn and others.
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
#include "graphvizedgegraphicsitem.h"
#include "graphvizgraphicslayout.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtDebug>

#define NORMAL_PENWIDTH 2
#define SELECTED_PENWIDTH 3

GraphvizEdgeGraphicsItem::GraphvizEdgeGraphicsItem(
    QGraphicsItem *parent, QGraphicsLayoutItem *tail, QGraphicsLayoutItem *head,
    QString label)
  : QGraphicsItem(parent), _tail(tail), _head(head), _label(label),
    _parentLayout(0), _penStyle(Qt::SolidLine) {
}

GraphvizEdgeGraphicsItem::~GraphvizEdgeGraphicsItem() {
  //qDebug() << "~GraphvizEdgeGraphicsItem" << this;
  if (_parentLayout)
    _parentLayout->removeEdge(this);
}

void GraphvizEdgeGraphicsItem::setControlPoints(QList<QPointF> points) {
  // Qt 2D graphics framework (QGraphicsView / QPainterPath) supports
  // cubic Beziers but not all B-splines in general, which is theoricaly what
  // Graphviz ouput is made of.
  // However, for most Graphviz' -Gsplines= option values, produced coordinates
  // can be interpreted as a composite Bezier curve
  // see http://en.wikipedia.org/wiki/Composite_B%C3%A9zier_curve
  // This is what is done below.
  _controlPoints = points;
  int n = _controlPoints.size();
  //qDebug() << "*** computing for edge " << _label << n << this;
  if (n >= 4 && n % 3 == 1) {
    _path = QPainterPath(_controlPoints[0]);
    //qDebug() << "    origin" << _controlPoints[0];
    for (int i = 1; i < n; i += 3) {
      //qDebug() << "    cubic" << i << _controlPoints[i]
      //            <<  _controlPoints[i+1] << _controlPoints[i+2];
      _path.cubicTo(_controlPoints[i], _controlPoints[i+1],
          _controlPoints[i+2]);
    }
    _boundingRect = _path
        .boundingRect()
        .adjusted(-SELECTED_PENWIDTH, -SELECTED_PENWIDTH,
                  SELECTED_PENWIDTH, SELECTED_PENWIDTH);
    //qDebug() << "    result " << _path;
    //qDebug() << "    bounding rect " << _boundingRect;
  } else {
    qDebug() << "GraphvizEdgeGraphicsItem::setControlPoints called with an "
                "unsupported number of control points should be = 3n+1 with "
                "n >= 1";
    _path = QPainterPath();
    _boundingRect = QRectF();
  }
}

void GraphvizEdgeGraphicsItem::setLabelPos(QPointF pos) {
  // LATER compute path (and bb)
  _labelPos = pos;
}

QRectF GraphvizEdgeGraphicsItem::boundingRect() const {
  return _boundingRect;
}

void GraphvizEdgeGraphicsItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  QPen pen;
  pen.setWidth(isSelected() ? SELECTED_PENWIDTH : NORMAL_PENWIDTH);
  //painter->setPen(Qt::red);
  //painter->drawRect(boundingRect());
  pen.setStyle(penStyle());
  painter->setPen(pen);
  painter->drawPath(_path);
  //painter->setPen(Qt::yellow);
  //painter->drawEllipse(_labelPos, 5, 5);
  // TODO head and tails decorations (arrows)
  // TODO color and color gradient
  // LATER draw label
}

QDebug operator<<(QDebug dbg, const GraphvizEdgeGraphicsItem &edge) {
  dbg.nospace() << "edge::{ tail: " << QString::number((quint64)edge.tail(), 16)
                << " head: " << QString::number((quint64)edge.head(), 16)
                << " label: " << edge.label() << " labelPos: "
                << edge.labelPos() << " controlPoints: { ";
  foreach (QPointF point, edge.controlPoints())
    dbg.nospace() << point;
  dbg.nospace() << " } }";
  return dbg.space();
}
