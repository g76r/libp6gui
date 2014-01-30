#include "tool.h"
#include "documentmanager.h"
#include "toolactionwrapper.h"
#include "perspectivewidget.h"

Tool::Tool(DocumentManager *parent, const QString id) : QObject(0),
  _documentManager(parent), _id(id), _label(tr("Tool")), _enabled(true),
  _action(new ToolActionWrapper(this)) {
  connect(parent->targetManager(), SIGNAL(targetChanged(TargetManager::TargetType,PerspectiveWidget*,QStringList)),
          this, SLOT(targetChanged(TargetManager::TargetType,PerspectiveWidget*,QStringList)));
  connect(this, SIGNAL(changed()), _action, SIGNAL(changed()));
  connect(_action, SIGNAL(triggered()), this, SLOT(trigger()));
}

Tool::~Tool() {
}

QPointer<DocumentManager> Tool::documentManager() const {
  return _documentManager;
}

void Tool::setLabel(QString label) {
  if (_label != label) {
    _label = label;
    _action->setIconText(label);
    emit changed();
  }
}

void Tool::setIcon(QIcon icon) {
  _icon = icon;
  _action->setIcon(icon);
  emit changed();
}

void Tool::setEnabled(bool enabled) {
  if (_enabled != enabled) {
    _enabled = enabled;
    _action->setEnabled(enabled);
    emit changed();
  }
}

void Tool::targetChanged(TargetManager::TargetType targetType,
                         PerspectiveWidget *perspectiveWidget,
                         QStringList itemIds) {
  Q_UNUSED(targetType)
  Q_UNUSED(perspectiveWidget)
  Q_UNUSED(itemIds)
  // default: don't care target
}

void Tool::trigger() {
  // default: do nothing
  // LATER take care of last Tool triggered for F4 handling
  emit triggered();
}
