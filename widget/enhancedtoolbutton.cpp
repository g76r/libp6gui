#include "enhancedtoolbutton.h"
#include <QAction>
#include <QTimer>

EnhancedToolButton::EnhancedToolButton(QWidget *parent) : QToolButton(parent) {
}

void EnhancedToolButton::setDefaultAction(
    QAction *action, bool setRichTooltip) {
  Q_ASSERT(action != 0);
  if (setRichTooltip && !action->shortcut().isEmpty())
    action->setToolTip(action->text()+" ("+action->shortcut().toString()+")");
  QToolButton::setDefaultAction(action);
  connect(action, &QAction::triggered,
          this, &EnhancedToolButton::fakeAnimateClick);
}

void EnhancedToolButton::fakeAnimateClick() {
  switch(_triggerAnimationMethod) {
  case AlterStyleSheet:
    setStyleSheet("QToolButton { background: palette(light); }");
    repaint();
    QTimer::singleShot(100, this, SLOT(endOfFakeAnimateClick()));
    break;
  case SetDown:
    if (!isDown()) {
      setDown(true);
      repaint();
      QTimer::singleShot(100, this, SLOT(endOfFakeAnimateClick()));
    }
    break;
  case Disabled:
    ;
  }
}

void EnhancedToolButton::endOfFakeAnimateClick() {
  switch(_triggerAnimationMethod) {
  case AlterStyleSheet:
    setStyleSheet("");
    repaint();
    break;
  case SetDown:
    setDown(false);
    repaint();
    break;
  case Disabled:
    ;
  }
}
