#ifndef POPCLONEACTION_H
#define POPCLONEACTION_H

#include "libh6ncsu_global.h"
#include <QAction>
#include "perspectivewidget.h"

/** Calls popClone() on parent PerspectiveWidget. */
class LIBH6NCSUSHARED_EXPORT PopCloneAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(PopCloneAction)

public:
  PopCloneAction(PerspectiveWidget *parent);
};

#endif // POPCLONEACTION_H
