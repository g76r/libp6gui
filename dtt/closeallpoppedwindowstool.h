#ifndef CLOSEALLSUBWINDOWSTOOL_H
#define CLOSEALLSUBWINDOWSTOOL_H

#include "tool.h"
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT CloseAllPoppedWindowsTool : public Tool {
  Q_OBJECT
  Q_DISABLE_COPY(CloseAllPoppedWindowsTool)
public:
  explicit CloseAllPoppedWindowsTool(DocumentManager *parent);
  void trigger(TargetManager::TargetType targetType);
};

#endif // CLOSEALLSUBWINDOWSTOOL_H
