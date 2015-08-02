#ifndef CLOSEALLSUBWINDOWSACTION_H
#define CLOSEALLSUBWINDOWSACTION_H

#include "dtpaction.h"
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT CloseAllPoppedWindowsAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(CloseAllPoppedWindowsAction)

public:
  explicit CloseAllPoppedWindowsAction(DtpDocumentManager *documentManager);
};

#endif // CLOSEALLSUBWINDOWSACTION_H
