#ifndef TOOLACTIONWRAPPER_H
#define TOOLACTIONWRAPPER_H

#include <QAction>
#include "tool.h"
#include "libh6ncsu_global.h"

/** Utility class used internaly by Tool class */
class LIBH6NCSUSHARED_EXPORT ToolActionWrapper : public QAction {
  Q_OBJECT
private:
  QPointer<Tool> _tool;

public:
  explicit ToolActionWrapper(Tool *tool);

public slots:

};

#endif // TOOLACTIONWRAPPER_H
