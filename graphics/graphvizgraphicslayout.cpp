#include "graphvizgraphicslayout.h"
#include <QtDebug>
#include <QProcess>
#include <QBuffer>

#define DPI 72.0

/* Implementation note:
 * This is a custom QGraphicsLayout.
 *
 * QGraphicsLayout documentation is not enough, but Qt's flow layout example
 * give more details about the way to implement QGraphicsLayout methods.
 * See here:
 * https://qt.gitorious.org/qt/qtbase/source/stable:examples/widgets/graphicsview/flowlayout
 * (or in any Qt distribution)
 *
 * In addition, a layout impl MUST unparent all child QGraphicsLayoutItem within
 * its destructor, and delete them if they are ownedByLayout().
 */

GraphvizGraphicsLayout::GraphvizGraphicsLayout(QGraphicsLayoutItem *parent)
  : QGraphicsLayout(parent), _graphvizCommand("graphviz/dot"),
    _isLayoutValid(false) {
}

GraphvizGraphicsLayout::~GraphvizGraphicsLayout() {
  qDebug() << "~GraphvizGraphicsLayout" << this << ownedByLayout() << parentLayoutItem();
  for (int i = count() - 1; i >= 0; --i) {
      QGraphicsLayoutItem *item = itemAt(i);
      // The following lines can be removed, but this removes the item
      // from the layout more efficiently than the implementation of
      // ~QGraphicsLayoutItem.
      removeAt(i);
      if (item) {
          item->setParentLayoutItem(0);
          if (item->ownedByLayout())
              delete item;
      }
  }
  foreach (GraphvizEdgeGraphicsItem *edge, _edges)
    edge->setParentLayout(0);
}

int GraphvizGraphicsLayout::count() const {
  return _nodes.size();
}

QGraphicsLayoutItem *GraphvizGraphicsLayout::itemAt(int i) const {
  return _nodes.value(i);
}

void GraphvizGraphicsLayout::addNode(QGraphicsLayoutItem *child, QString name) {
  if (child) {
    QGraphicsLayout::addChildLayoutItem(child);
    _nodes.append(child);
    // TODO remove " from node names
    if (!name.isEmpty()) {
      _nodesNames.insert(child, name);
      _nodesByName.insert(name, child);
      invalidate();
    }
  }
}

void GraphvizGraphicsLayout::addEdge(GraphvizEdgeGraphicsItem *child) {
  if (child) {
    if (child->parentLayout()) {
      qDebug() << "Cannot add a GraphvizEdgeGraphicsItem to a "
                  "GraphvizGraphicsLayout if it already belong to another "
                  "layout:" << child << child->parentLayout();
      return;
    }
    // FIXME reparent, as QGraphicsLayout::addChildLayoutItem would
    // however GraphvizEdgeGraphicsItem is not a QGraphicsLayoutItem and thus should not be child of a layout but of the topmost parent of the layout hierarchy
    // therefore maybe this cannot be done here in a layout
    _edges.append(child);
    child->setParentLayout(this);
    invalidate();
  }
}

GraphvizEdgeGraphicsItem *GraphvizGraphicsLayout::createEdge(
    QGraphicsItem *parent, QGraphicsLayoutItem *tail,
    QGraphicsLayoutItem *head, QString label) {
  if (tail && head) {
    GraphvizEdgeGraphicsItem *edge =
        new GraphvizEdgeGraphicsItem(parent, tail, head, label);
    addEdge(edge);
    return edge;
  }
  return 0;
}

GraphvizEdgeGraphicsItem *GraphvizGraphicsLayout::createEdge(
    QGraphicsItem *parent, QString tailName, QString headName,
    QString label) {
  return createEdge(parent, _nodesByName.value(tailName),
                    _nodesByName.value(headName), label);
}

void GraphvizGraphicsLayout::removeAt(int i) {
  QGraphicsLayoutItem *child = _nodes.value(i);
  if (child) {
    _nodes.removeAt(i);
    QString nodeName = _nodesNames.value(child);
    if (!nodeName.isEmpty()) {
      _nodesNames.remove(child);
      _nodesByName.remove(nodeName);
      // FIXME remove edges to and from this node
    }
  }
  invalidate();
}

void GraphvizGraphicsLayout::removeNode(QGraphicsLayoutItem *child) {
  if (child) {
    _nodes.removeAll(child);
    _nodesByName.remove(_nodesNames.value(child));
    _nodesNames.remove(child);
  }
}

void GraphvizGraphicsLayout::removeNode(QString name) {
  QGraphicsLayoutItem *child =_nodesByName.value(name);
  if (child) {
    _nodes.removeAll(child);
    _nodesByName.remove(name);
    _nodesNames.remove(child);
  }
}

void GraphvizGraphicsLayout::removeEdge(GraphvizEdgeGraphicsItem *child) {
  _edges.removeAll(child);
  invalidate();
}

void GraphvizGraphicsLayout::invalidate() {
  _isLayoutValid = false;
  QGraphicsLayout::invalidate();
}

QSizeF GraphvizGraphicsLayout::sizeHint(Qt::SizeHint which,
                                        const QSizeF &constraint) const {
  Q_UNUSED(constraint)
  if (which == Qt::PreferredSize) {
    const_cast<GraphvizGraphicsLayout*>(this)->computeLayout();
    //qDebug() << "******** GraphvizGraphicsLayout::sizeHint(preferred)"
    //         << _currentLayout._graphSize;
    return _currentLayout._graphSize;
  }
  return QSizeF();
}

void GraphvizGraphicsLayout::setGeometry(const QRectF &rect) {
  // FIXME prepareGeometryChange ? update ?
  qDebug() << "******** GraphvizGraphicsLayout::setGeometry" << rect
           << _isLayoutValid;
  QGraphicsLayout::setGeometry(rect);
  applyLayout();
}

void GraphvizGraphicsLayout::computeLayout() {
  if (_isLayoutValid) {
    qDebug() << "no need to compute layout";
    return;
  }
  QString inGraph = "graph g {\n";
  foreach (QGraphicsLayoutItem *node, _nodes) {
    QString nodeName = _nodesNames.value(node);
    if (!nodeName.isEmpty()) { // should always be true
      inGraph += "  \""+nodeName+"\" [";
      // TODO setting width and height should be an option
      qreal width = node->preferredWidth()/DPI,
          height = node->preferredHeight()/DPI;
      if (width > 0)
        inGraph += "width="+QString::number(width)+" ";
      if (height > 0)
        inGraph += "height="+QString::number(height)+" ";
      inGraph += "]\n";
    }
  }
  foreach (GraphvizEdgeGraphicsItem *edge, _edges) {
    QString tailNodeName = _nodesNames.value(edge->tail()),
        headNodeName = _nodesNames.value(edge->head());
    if (!tailNodeName.isEmpty() && !headNodeName.isEmpty()) {
      // MAYDO make -- or -> parametrized
      inGraph += "  \""+tailNodeName+"\" -- \""+headNodeName+"\" [label=\""
          +edge->label()+"\"]\n";
    }
  }
  inGraph.append("}\n");
  //qDebug() << "input graphviz graph: " << inGraph;
  // TODO set environment to ensure UTF8
  // -Gsplines must be "spline" or "polyline", avoid "curved" since the produced
  // B-splines cannot be converted to cubic Bezier for QPainterPath support
  QByteArray outGraph = execGraphivzEngine(
        _graphvizCommand, QStringList() << "-Tplain"
        //<< "-Granksep=1" // TODO make ranksep a parameter
        << "-Gsplines=spline"
        << "-Nshape=box" // TODO make box shape a param (used for edges cliping)
        << "-Nstyle=rounded" // TODO same
        << "-Edir=none", // make edge ends the closer possible to nodes
        //<< "-Earrowhead=none" << "-Earrowtail=none",
        inGraph.toUtf8());
  Layout newLayout = parseGraphvizOutput(outGraph);
  _currentLayout = newLayout;
  _isLayoutValid = true;
  qDebug() << "computed layout:" << newLayout;
}

void GraphvizGraphicsLayout::applyLayout() {
  if (!_isLayoutValid)
    computeLayout();
  // we need to translate graphviz coordinates into QGraphicsWidget coordinates:
  // - graphviz uses PostScript coordinates conventions (affine frame) rather
  //   than screen top-left origin conventions
  // - graphivz node position are those of its center, not top-left corner
  // - the whole diagram must be translated to match margins
  foreach(QGraphicsLayoutItem *node, _currentLayout._nodesPos.keys()) {
    QRectF geometry(fromLayoutCoord(_currentLayout._nodesPos.value(node)),
                    _currentLayout._nodesSize.value(node));
    //qDebug() << "*** applygeometry to node:" << _nodesNames.value(node)
    //         << geometry;
    node->setGeometry(geometry);
  }
  foreach (GraphvizEdgeGraphicsItem *edge, _currentLayout._edgePoints.keys()) {
    edge->setLabelPos(
          fromLayoutCoord(_currentLayout._edgeLabelsPos.value(edge)));
    QList<QPointF> points;
    foreach (QPointF point, _currentLayout._edgePoints.value(edge))
      points << fromLayoutCoord(point);
    edge->setControlPoints(points);
  }
}

QPointF GraphvizGraphicsLayout::fromPsCoord(QPointF psPoint, QSizeF graphSize) {
  return QPointF(psPoint.x(), graphSize.height() - psPoint.y());
}

QPointF GraphvizGraphicsLayout::fromCenterCoord(QPointF center, QSizeF size) {
  return QPointF(center.x() - size.width()/2, center.y() - size.height()/2);
}

QPointF GraphvizGraphicsLayout::fromLayoutCoord(QPointF point) const {
  return point + geometry().topLeft();
}

//QPointF GraphvizGraphicsLayout::fromGraphvizCoord(
//    QPointF psPoint, QSizeF graphSize) const {
//  QPointF point(psPoint.x(), graphSize.height() - psPoint.y()); // from PS
//  point += geometry().topLeft(); // to parent QGraphicsItem-local coordinates
//  return point;
//}

#include <QTimer> // FIXME
#include <unistd.h>
QByteArray GraphvizGraphicsLayout::execGraphivzEngine(
    QString command, QStringList args, QByteArray input) {
  // FIXME this is too slow
  QProcess process;
  qDebug() << "starting graphviz rendering with this data: "
           << QString::fromUtf8(input);
  process.start(command, args);
  if (!process.waitForStarted(1000)) {
    qDebug() << "cannot start graphviz processor: "
             << process.error() << " " << process.errorString();
    return QByteArray();
  }
  qint64 written = process.write(input);
  // TODO display error to user, and have a timeout
  if (written != input.size())
    qDebug()<< "cannot write to graphviz processor "
            << written << " " << input.size() << " "
            << process.error() << " " << process.errorString();
  process.closeWriteChannel();
  QByteArray output, error;
  /*QTimer timer;
  timer.setSingleShot(true);
  timer.start(1000);*/
  process.waitForFinished(1000);
  //::usleep(3000000);
  do {
    // TODO optimize this loop
    process.setReadChannel(QProcess::StandardOutput);
    process.waitForReadyRead(1000);
    QByteArray ba = process.readAll();
    qDebug() << "read" << ba.size() << "bytes on output";
    output.append(ba);
    process.setReadChannel(QProcess::StandardError);
    process.waitForReadyRead(1000);
    ba = process.readAll();
    qDebug() << "read" << ba.size() << "bytes on error";
    error.append(ba);
  } while (false && process.state() == QProcess::Running /*&& timer.isActive()*/);
  qDebug() << "graphviz output: " << QString::fromUtf8(output) << "|";
  qDebug() << "graphviz error: " << QString::fromUtf8(error) << "|";
  if (process.exitStatus() == QProcess::NormalExit
      && process.exitCode() == 0) {
    return output;
  } else {
    qDebug() << "graphviz process status:" << process.exitStatus()
             << "exit code:" << process.exitCode();
  }
  return QByteArray();
}

GraphvizGraphicsLayout::Layout GraphvizGraphicsLayout::parseGraphvizOutput(
    QByteArray output) const {
  // parses graphviz' plain format, see specification here:
  // http://graphviz.org/content/output-formats#dplain
  QBuffer buf(&output);
  buf.open(QIODevice::ReadOnly);
  QByteArray rawLine;
  Layout layout;
  qreal scale = 1.0;
  int nodeIndex = 0, edgeIndex = 0;
  while (!(rawLine = buf.readLine()).isEmpty()) {
    QString line = QString::fromUtf8(rawLine);
    QStringList fields = line.split(' ');
    if (fields.size() < 4)
      continue; // LATER this should not occur and should be an error
    if (fields[0] == "graph") {
      scale = fields[1].toDouble()*DPI;
      layout._graphSize.setWidth(fields[2].toDouble()*scale);
      layout._graphSize.setHeight(fields[3].toDouble()*scale);
    } else if (fields[0] == "node") {
      if (nodeIndex >= _nodes.size())
        continue; // LATER should be an error
      if (fields.size() < 6)
        continue; // LATER should be an error
      QPointF pos(fields[2].toDouble()*scale, fields[3].toDouble()*scale);
      QSizeF size(fields[4].toDouble()*scale, fields[5].toDouble()*scale);
      layout._nodesPos.insert(_nodes[nodeIndex],
          fromCenterCoord(fromPsCoord(pos, layout._graphSize), size));
      layout._nodesSize.insert(_nodes[nodeIndex++], size);
    } else if (fields[0] == "edge") {
      if (edgeIndex >= _edges.size())
        continue; // LATER should be an error
      int n = fields[3].toInt();
      if (fields.size() <= 4+2*n+4)
        continue; // LATER should be an error
      QList<QPointF> points;
      for (int i = 0; i < n; ++i) {
        QPointF point(fields[4+2*i].toDouble()*scale,
            fields[4+2*i+1].toDouble()*scale);
        points << fromPsCoord(point, layout._graphSize);
      }
      layout._edgePoints.insert(_edges[edgeIndex], points);
      // TODO process label pos only if present
      QPointF pos(fields[4+2*n+3].toDouble()*scale,
          fields[4+2*n+4].toDouble()*scale);
      layout._edgeLabelsPos.insert(_edges[edgeIndex++],
          fromPsCoord(pos, layout._graphSize));
    } else if (fields[0] == "stop") {
      break;
    } else {
      // LATER should be an error
    }
  }
  return layout;
}

QDebug operator<<(QDebug dbg, const GraphvizGraphicsLayout::Layout &layout) {
  dbg.nospace() << "{ graphSize: " << layout._graphSize << "; nodesPos: { ";
  foreach (QPointF pos, layout._nodesPos.values())
    dbg.nospace() << pos;
  dbg.nospace() << " }; nodesSize: { ";
  foreach (QSizeF size, layout._nodesSize.values())
    dbg.nospace() << size;
  dbg.nospace() << " }; edgeLabelsPos: { ";
  foreach (QPointF pos, layout._edgeLabelsPos.values())
    dbg.nospace() << pos;
  dbg.nospace() << " }; edgePoints: { ";
  foreach (GraphvizEdgeGraphicsItem *edge, layout._edgePoints.keys()) {
    QList<QPointF> points = layout._edgePoints.value(edge);
    dbg.nospace() << "{ ";
    foreach (QPointF point, points)
      dbg.nospace() << point;
    dbg.nospace() << " } ";
  }
  dbg.nospace() << " }; }";
  return dbg.space();
}
