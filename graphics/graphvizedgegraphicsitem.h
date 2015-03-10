#ifndef GRAPHVIZEDGEGRAPHICSITEM_H
#define GRAPHVIZEDGEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsLayoutItem>
#include "libh6ncsu_global.h"
#include <QPainterPath>
#include <QRectF>

class GraphvizGraphicsLayout;

/** Graphics item fitting with Graphviz' edges model: B-splines (actually
 * linked cubic Beziers) with a floating label.
 * @see GraphvizGraphicsLayout
 */
class LIBH6NCSUSHARED_EXPORT GraphvizEdgeGraphicsItem : public QGraphicsItem {
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
  QRectF boundingRect() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);
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

QDebug LIBH6NCSUSHARED_EXPORT operator<<(
    QDebug dbg, const GraphvizEdgeGraphicsItem &edge);

#endif // GRAPHVIZEDGEGRAPHICSITEM_H
