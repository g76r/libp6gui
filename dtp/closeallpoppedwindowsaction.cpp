#include "closeallpoppedwindowsaction.h"
#include "dtpdocumentmanager.h"
#include "perspectivewidget.h"
#include <QApplication>

CloseAllPoppedWindowsAction::CloseAllPoppedWindowsAction(
    DtpDocumentManager *documentManager)
  : DtpAction(documentManager, "closeAllPoppedWindows") {
  setIcon(QIcon(":fa/desktop.svg"));
  setText(tr("Close All Popped Windows"));
  setEnabled(true);
  connect(this, &QAction::triggered, []() {
    foreach (QWidget *w, QApplication::topLevelWidgets()) {
      auto pw = qobject_cast<PerspectiveWidget*>(w);
      if (pw)
        pw->close();
    }
  });
}
