#ifndef GRAPHVIZGRAPHICSLAYOUT_H
#define GRAPHVIZGRAPHICSLAYOUT_H

#include <QGraphicsLayout>
#include "graphvizedgegraphicsitem.h"
#include <QDebug>
#include "libh6ncsu_global.h"

/** Layout for using Graphviz to layout two item kinds, graph nodes being
 * represented by regular QGraphicsLayoutItems and edges being represented
 * by ad-hoc GraphvizEdgeGraphicsItem objects that are QGraphicsItems but not
 * QGraphicsLayoutItems.
 * @see GraphvizEdgeGraphicsItem
 */
class LIBH6NCSUSHARED_EXPORT GraphvizGraphicsLayout : public QGraphicsLayout {
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
  int count() const;
  /** @return ith node (edges are not accessible this way) */
  QGraphicsLayoutItem *itemAt(int i) const;
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
  void removeAt(int index);
  void removeNode(QGraphicsLayoutItem *child);
  void removeNode(QString name);
  void removeEdge(GraphvizEdgeGraphicsItem *child);
  void setGeometry(const QRectF &rect);
  void invalidate();

protected:
  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

private:
  void computeLayout();
  static inline QByteArray execGraphivzEngine(QString command, QStringList args,
                                              QByteArray input);
  inline Layout parseGraphvizOutput(QByteArray output) const;
  inline void applyLayout();
  /** Translate from postscript affine frame to QGraphicsWidget screen frame. */
  static inline QPointF fromPsCoord(QPointF psPoint, QSizeF graphSize);
  /** Translate from center node pos to top-left corner widget pos. */
  static inline QPointF fromCenterCoord(QPointF center, QSizeF size);
  /** Translate from layout-local coords to parent QGraphicsItem coords.
    * Does nothing when the layout is the only one in the QGraphicsWidget, but
    * is needed if several layouts are embeded one into another. */
  inline QPointF fromLayoutCoord(QPointF point) const;
};

QDebug LIBH6NCSUSHARED_EXPORT operator<<(
    QDebug dbg, const GraphvizGraphicsLayout::Layout &layout);

#endif // GRAPHVIZGRAPHICSLAYOUT_H
