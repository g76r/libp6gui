#include "targetkeepingmenu.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>

TargetKeepingMenu::TargetKeepingMenu(PerspectiveWidget *parent)
  : QMenu(parent) {
}

void TargetKeepingMenu::popup(const QPoint &pos, QAction *atAction) {
  if (!_documentManager) {
    auto *perspectiveWidget = qobject_cast<PerspectiveWidget*>(parent());
    if (perspectiveWidget)
      _documentManager = perspectiveWidget->documentManager();
    else
      qWarning() << "TargetKeepingMenu::popup() called with null parent";
  }
  if (_documentManager) {
    _primaryWidget = _documentManager->targetManager()->targetWidget();
    _primaryItems = _documentManager->targetManager()->targetItems();
  } else {
    _primaryWidget = 0;
    _primaryItems.clear();
  }
  QMenu::popup(pos, atAction);
}

// LATER should also override exec() in addition to popup() ?

void TargetKeepingMenu::enterEvent(QEvent *event) {
  QMenu::enterEvent(event);
  // setting the target must be done in enterEvent() rather than showEvent()
  // because leaveEvent() is called on the perspective widget *after*
  // showEvent() is called on the QMenu
  if (_documentManager) {
    _documentManager->targetManager()->setTarget(_primaryWidget, _primaryItems);
  }
}
