#include "widgetutils.h"
#include <QWidget>
#include <QStackedWidget>
#include <QTabWidget>

void WidgetUtils::activateAndShowWindow(QWidget *widget) {
  Q_ASSERT(widget != 0);
  widget->window()->setWindowState(
        (widget->window()->windowState()
         & ~Qt::WindowMinimized) | Qt::WindowActive);
  widget->window()->raise(); // for X11
  widget->activateWindow();
}

void WidgetUtils::ensureWidgetIsVisible(QWidget *widget) {
  Q_ASSERT(widget != 0);
  for (QWidget *child = widget, *parent = widget->parentWidget(); parent;
       child = parent, parent = parent->parentWidget()) {
    auto *stackedWidget = qobject_cast<QStackedWidget*>(parent);
    if (stackedWidget) {
      stackedWidget->setCurrentWidget(child);
      continue;
    }
    auto *tabWidget = qobject_cast<QTabWidget*>(parent);
    if (tabWidget) {
      tabWidget->setCurrentWidget(child);
      continue;
    }
    // LATER support other containers: QToolbox, disabled QGroupBox, folded QSplitter
  }
  activateAndShowWindow(widget);
}
