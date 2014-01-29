#ifndef ENHANCEDGRAPHICSVIEW_H
#define ENHANCEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include "libh6ncsu_global.h"

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
