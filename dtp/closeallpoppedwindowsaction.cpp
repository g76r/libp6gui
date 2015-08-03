#include "closeallpoppedwindowsaction.h"
#include "dtpdocumentmanager.h"
#include "perspectivewidget.h"
#include <QApplication>

CloseAllPoppedWindowsAction::CloseAllPoppedWindowsAction(QObject *parent)
  : QAction(QIcon(":fa/desktop.svg"), tr("Close All Popped Windows"), parent) {
  connect(this, &QAction::triggered, []() {
    foreach (QWidget *w, QApplication::topLevelWidgets()) {
      auto pw = qobject_cast<PerspectiveWidget*>(w);
      if (pw)
        pw->close();
    }
  });
}
