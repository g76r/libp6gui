#ifndef CLOSEALLSUBWINDOWSACTION_H
#define CLOSEALLSUBWINDOWSACTION_H

#include "libh6ncsu_global.h"
#include <QAction>

/** Close all PerspectiveWidget top level windows. */
class LIBH6NCSUSHARED_EXPORT CloseAllPoppedWindowsAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(CloseAllPoppedWindowsAction)

public:
  explicit CloseAllPoppedWindowsAction(QObject *parent);
};

#endif // CLOSEALLSUBWINDOWSACTION_H
