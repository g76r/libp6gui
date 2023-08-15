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
#ifndef GRAPHVIZEDGEGRAPHICSITEM_H
#define GRAPHVIZEDGEGRAPHICSITEM_H

#include "libp6gui_global.h"
#include <QGraphicsItem>
#include <QGraphicsLayoutItem>

class GraphvizGraphicsLayout;

/** Graphics item fitting with Graphviz' edges model: B-splines (actually
 * linked cubic Beziers) with a floating label.
 * @see GraphvizGraphicsLayout
 */
class LIBP6GUISHARED_EXPORT GraphvizEdgeGraphicsItem : public QGraphicsItem {
  QGraphicsLayoutItem *_tail, *_head;
  QString _label;
  QPointF _labelPos;
  QList<QPointF> _controlPoints;
  GraphvizGraphicsLayout *_parentLayout;
  QPainterPath _path;
  QRectF _boundingRect;
  Qt::PenStyle _penStyle;

public:
  GraphvizEdgeGraphicsItem(QGraphicsItem *parent, QGraphicsLayoutItem *tail,
                           QGraphicsLayoutItem *head,
                           QString label = QString());
  ~GraphvizEdgeGraphicsItem();
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  QGraphicsLayoutItem *tail() const { return _tail; }
  QGraphicsLayoutItem *head() const { return _head; }
  QString label() const { return _label; }
  void setLabel(QString label) { _label = label; }
  QPointF labelPos() const { return _labelPos; }
  void setLabelPos(QPointF pos);
  QList<QPointF> controlPoints() const { return _controlPoints; }
  void setControlPoints(QList<QPointF> points);
  GraphvizGraphicsLayout *parentLayout() const { return _parentLayout; }
  void setParentLayout(GraphvizGraphicsLayout *l) { _parentLayout = l; }
  Qt::PenStyle penStyle() const { return _penStyle; }
  void setPenStyle(Qt::PenStyle penStyle) { _penStyle = penStyle; }
};

QDebug LIBP6GUISHARED_EXPORT operator<<(
    QDebug dbg, const GraphvizEdgeGraphicsItem &edge);

#endif // GRAPHVIZEDGEGRAPHICSITEM_H
