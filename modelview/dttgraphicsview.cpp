#include "dttgraphicsview.h"
#include "dtt/targetmanager.h"
#include "dttgraphicsscene.h"

DttGraphicsView::DttGraphicsView(QWidget *parent)
  : EnhancedGraphicsView(parent) {
  setFocusPolicy(Qt::StrongFocus);
}

void DttGraphicsView::setScene(QGraphicsScene *scene) {
  setMouseTracking(scene);
  EnhancedGraphicsView::setScene(scene);
  DttGraphicsScene *dgs = qobject_cast<DttGraphicsScene*>(scene);
  _perspectiveWidget = dgs ? dgs->perspectiveWidget() : 0;
  if (dgs && hasFocus()) {
    TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
    if (tm)
      tm->setTarget(_perspectiveWidget, dgs->selectedItemsIds());
  }
}

void DttGraphicsView::focusInEvent(QFocusEvent *event) {
  EnhancedGraphicsView::focusInEvent(event);
  DttGraphicsScene *dgs = qobject_cast<DttGraphicsScene*>(scene());
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (dgs && tm)
    tm->setTarget(_perspectiveWidget, dgs->selectedItemsIds());
}

void DttGraphicsView::focusOutEvent(QFocusEvent *event) {
  EnhancedGraphicsView::focusOutEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget();
}

void DttGraphicsView::enterEvent(QEvent *event) {
  EnhancedGraphicsView::enterEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget);
}

void DttGraphicsView::leaveEvent(QEvent *event) {
  EnhancedGraphicsView::leaveEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}
