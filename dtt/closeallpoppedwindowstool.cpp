#include "closeallpoppedwindowstool.h"
#include "dttdocumentmanager.h"
#include "perspectivewidget.h"
#include <QApplication>

CloseAllPoppedWindowsTool::CloseAllPoppedWindowsTool(DttDocumentManager *parent)
  : Tool(parent, "closeAllPoppedWindows") {
  setIcon(QIcon(":fa/desktop.svg"));
  setLabel(tr("Close All Popped Windows"));
  setEnabled(true);
}

void CloseAllPoppedWindowsTool::trigger(TargetManager::TargetType targetType) {
  foreach (QWidget *w, QApplication::topLevelWidgets()) {
    auto pw = qobject_cast<PerspectiveWidget*>(w);
    if (pw)
      pw->close();
  }
  Tool::trigger(targetType);
}
