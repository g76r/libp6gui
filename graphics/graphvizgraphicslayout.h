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
#ifndef GRAPHVIZGRAPHICSLAYOUT_H
#define GRAPHVIZGRAPHICSLAYOUT_H

#include "graphvizedgegraphicsitem.h"
#include <QGraphicsLayout>

/** Layout for using Graphviz to layout two item kinds, graph nodes being
 * represented by regular QGraphicsLayoutItems and edges being represented
 * by ad-hoc GraphvizEdgeGraphicsItem objects that are QGraphicsItems but not
 * QGraphicsLayoutItems.
 * @see GraphvizEdgeGraphicsItem
 */
class LIBP6GUISHARED_EXPORT GraphvizGraphicsLayout : public QGraphicsLayout {
public:
  class Layout {
  public:
    QSizeF _graphSize;
    QHash<QGraphicsLayoutItem*,QPointF> _nodesPos;
    QHash<QGraphicsLayoutItem*,QSizeF> _nodesSize;
    QHash<GraphvizEdgeGraphicsItem*,QPointF> _edgeLabelsPos;
    QHash<GraphvizEdgeGraphicsItem*,QList<QPointF> > _edgePoints;
    bool isNull() const { return _graphSize.isNull(); }
  };

private:
  QList<QGraphicsLayoutItem*> _nodes;
  QList<GraphvizEdgeGraphicsItem*> _edges;
  QHash<QGraphicsLayoutItem*, QString> _nodesNames;
  QHash<QString, QGraphicsLayoutItem*> _nodesByName;
  QString _graphvizCommand;
  Layout _currentLayout;
  bool _isLayoutValid;

public:
  explicit GraphvizGraphicsLayout(QGraphicsLayoutItem *parent = 0);
  ~GraphvizGraphicsLayout();
  /** @return nodes count (not counting edges) */
  int count() const override;
  /** @return ith node (edges are not accessible this way) */
  QGraphicsLayoutItem *itemAt(int i) const override;
  /** Add a QGraphicsLayoutItem that will be layouted as a node by graphviz
   * layout algorithm. */
  void addNode(QGraphicsLayoutItem *child, QString name);
  /** Add a graphivz edge, which will be layouted as an edge by graphviz
   * layout algorithm and layouted by GraphvizGraphicsLayout but *is not*
   * a QGraphicsLayoutItem nor a QGraphicsWidget and is not layouted in the
   * Qt meaning, especially its bounding box may (and often will) collides
   * with edges bouding boxes. */
  void addEdge(GraphvizEdgeGraphicsItem *child);
  /** Create and add edge but if tail or head are null, in this case do nothing
   * and return null.
   * @see addEdge() */
  GraphvizEdgeGraphicsItem *createEdge(
      QGraphicsItem *parent, QGraphicsLayoutItem *tail,
      QGraphicsLayoutItem *head, QString label = QString());
  /** Create and add edge but if tail or head have not known / have not been
   * added prior to edge, in this case do nothing and return null.
   * @see addEdge() */
  GraphvizEdgeGraphicsItem *createEdge(
      QGraphicsItem *parent, QString tailName, QString headName,
      QString label = QString());
  /** remove ith node (edges are not accessible this way) */
  void removeAt(int index) override;
  void removeNode(QGraphicsLayoutItem *child);
  void removeNode(QString name);
  void removeEdge(GraphvizEdgeGraphicsItem *child);
  void setGeometry(const QRectF &rect) override;
  void invalidate() override;

protected:
  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;

private:
  void computeLayout();
  static inline QByteArray execGraphivzEngine(QString command, QStringList args,
                                              QByteArray input);
  inline Layout parseGraphvizOutput(QByteArray output) const;
  inline void applyLayout();
  /** Translate from postscript affine frame to QGraphicsWidget screen frame. */
  static inline QPointF fromPsCoord(
      const QPointF &psPoint, const QSizeF &graphSize) {
    return QPointF(psPoint.x(), graphSize.height() - psPoint.y());
  }
  /** Translate from center node pos to top-left corner widget pos. */
  static inline QPointF fromCenterCoord(
      const QPointF &center, const QSizeF &size) {
    return QPointF(center.x() - size.width()/2, center.y() - size.height()/2);
  }
  /** Translate from layout-local coords to parent QGraphicsItem coords.
    * Does nothing when the layout is the only one in the QGraphicsWidget, but
    * is needed if several layouts are embeded one into another. */
  inline QPointF fromLayoutCoord(const QPointF &point) const {
    return point + geometry().topLeft();
  }
};

QDebug LIBP6GUISHARED_EXPORT operator<<(
    QDebug dbg, const GraphvizGraphicsLayout::Layout &layout);

#endif // GRAPHVIZGRAPHICSLAYOUT_H
