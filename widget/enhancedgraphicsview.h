#ifndef ENHANCEDGRAPHICSVIEW_H
#define ENHANCEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include "libh6ncsu_global.h"

/** Enhanced QListView
 *
 * Additionnal behaviours:
 * - bidirectional mouse grab scrolling on middle button
 * - zoom in and out on control + wheel up/wheel down
 *
 * Additionnal slots:
 * - fitAllInView(): zoom in or out to fit the current items bounding box
 * - zoomIn(), zoomOut(): zoom in or out by 20%
 */
class LIBH6NCSUSHARED_EXPORT EnhancedGraphicsView : public QGraphicsView {
  Q_OBJECT
private:
  bool _mouseDragScrolling, _mouseMoved;
  QPoint _lastPos;

public:
  explicit EnhancedGraphicsView(QWidget *parent);
  void wheelEvent(QWheelEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

public slots:
  void fitAllInView();
  void zoomIn();
  void zoomOut();
};

#endif // ENHANCEDGRAPHICSVIEW_H
