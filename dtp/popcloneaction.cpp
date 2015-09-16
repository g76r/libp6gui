#include "popcloneaction.h"

PopCloneAction::PopCloneAction(PerspectiveWidget *parent)
  : QAction(QIcon(":fa/external-link.svg"), tr("Pop Clone"), parent) {
  connect(this, &PopCloneAction::triggered,
          parent, &PerspectiveWidget::popClone);
}

