#include "dtpgraphicsview.h"
#include "dtp/targetmanager.h"
#include "dtpgraphicsscene.h"

DtpGraphicsView::DtpGraphicsView(QWidget *parent)
  : EnhancedGraphicsView(parent) {
  setFocusPolicy(Qt::StrongFocus);
}

void DtpGraphicsView::setScene(QGraphicsScene *scene) {
  setMouseTracking(scene);
  EnhancedGraphicsView::setScene(scene);
  DtpGraphicsScene *dgs = qobject_cast<DtpGraphicsScene*>(scene);
  _perspectiveWidget = dgs ? dgs->perspectiveWidget() : 0;
  if (dgs && hasFocus()) {
    TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
    if (tm)
      tm->setTarget(_perspectiveWidget, dgs->selectedItemsIds());
  }
}

void DtpGraphicsView::focusInEvent(QFocusEvent *event) {
  EnhancedGraphicsView::focusInEvent(event);
  DtpGraphicsScene *dgs = qobject_cast<DtpGraphicsScene*>(scene());
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (dgs && tm)
    tm->setTarget(_perspectiveWidget, dgs->selectedItemsIds());
}

void DtpGraphicsView::focusOutEvent(QFocusEvent *event) {
  EnhancedGraphicsView::focusOutEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget();
}

void DtpGraphicsView::enterEvent(QEvent *event) {
  EnhancedGraphicsView::enterEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget);
}

void DtpGraphicsView::leaveEvent(QEvent *event) {
  EnhancedGraphicsView::leaveEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}
