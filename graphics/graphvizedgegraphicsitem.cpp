#include "graphvizedgegraphicsitem.h"
#include "graphvizgraphicslayout.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtDebug>

GraphvizEdgeGraphicsItem::GraphvizEdgeGraphicsItem(
    QGraphicsItem *parent, QGraphicsLayoutItem *tail, QGraphicsLayoutItem *head,
    QString label)
  : QGraphicsItem(parent), _tail(tail), _head(head), _label(label),
    _parentLayout(0) {
}

GraphvizEdgeGraphicsItem::~GraphvizEdgeGraphicsItem() {
  //qDebug() << "~GraphvizEdgeGraphicsItem" << this;
  if (_parentLayout)
    _parentLayout->removeEdge(this);
}

void GraphvizEdgeGraphicsItem::setControlPoints(QList<QPointF> points) {
  // FIXME compute path (and bb)
  _controlPoints = points;
}

void GraphvizEdgeGraphicsItem::setLabelPos(QPointF pos) {
  // FIXME compute path (and bb)
  _labelPos = pos;
}

QRectF GraphvizEdgeGraphicsItem::boundingRect() const {
  // FIXME according to tail & head
  return QRectF();
}

void GraphvizEdgeGraphicsItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  // FIXME draw
  painter->setPen(Qt::red);
  painter->drawRect(boundingRect());
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
