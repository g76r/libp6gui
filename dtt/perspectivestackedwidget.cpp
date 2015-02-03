#include "perspectivestackedwidget.h"
#include "dttdocumentmanager.h"

PerspectiveStackedWidget::PerspectiveStackedWidget(PerspectiveWidget *parent)
  : QStackedWidget(parent), _perspectiveWidget(parent) {
}

void PerspectiveStackedWidget::enterEvent(QEvent *event) {
  QStackedWidget::enterEvent(event);
  setMouseoverTarget(true);
}

void PerspectiveStackedWidget::leaveEvent(QEvent *event) {
  QStackedWidget::leaveEvent(event);
  setMouseoverTarget(false);
}

void PerspectiveStackedWidget::perspectiveChanged() {
  if (underMouse())
    setMouseoverTarget(true);
}

void PerspectiveStackedWidget::setMouseoverTarget(bool set) {
  if (_perspectiveWidget) {
    DttDocumentManager *manager = _perspectiveWidget.data()->documentManager();
    if (manager) {
      if (set)
        manager->targetManager()->setTarget(TargetManager::MouseOverTarget,
                                            _perspectiveWidget);
      else
        manager->targetManager()->setTarget(TargetManager::MouseOverTarget,
                                            QPointer<PerspectiveWidget>());
    }
  }
}
