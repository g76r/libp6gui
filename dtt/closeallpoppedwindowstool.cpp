#include "closeallpoppedwindowstool.h"
#include "documentmanager.h"
#include "perspectivewidget.h"

CloseAllPoppedWindowsTool::CloseAllPoppedWindowsTool(DocumentManager *parent)
  : Tool(parent, "closeAllPoppedWindows") {
  setIcon(QIcon(":icons/Panda_001_Head_Cartoon.svg"));
  setLabel(tr("Close All Popped Windows"));
  setEnabled(true);
}

void CloseAllPoppedWindowsTool::trigger() {
  foreach (QWidget *w, documentManager()->perspectiveWidgets())
    if (!w->parent())
      w->close();
  Tool::trigger();
}
