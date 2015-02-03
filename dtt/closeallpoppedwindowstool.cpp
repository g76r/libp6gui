#include "closeallpoppedwindowstool.h"
#include "dttdocumentmanager.h"
#include "perspectivewidget.h"

CloseAllPoppedWindowsTool::CloseAllPoppedWindowsTool(DttDocumentManager *parent)
  : Tool(parent, "closeAllPoppedWindows") {
  setIcon(QIcon(":fa/desktop.svg"));
  setLabel(tr("Close All Popped Windows"));
  setEnabled(true);
}

void CloseAllPoppedWindowsTool::trigger(TargetManager::TargetType targetType) {
  foreach (QWidget *w, documentManager()->perspectiveWidgets())
    if (!w->parent())
      w->close();
  Tool::trigger(targetType);
}
