#include "tool.h"
#include "documentmanager.h"
#include "toolactionwrapper.h"
#include "perspectivewidget.h"

Tool::Tool(DocumentManager *parent, const QString id,
           QSet<TargetManager::TargetType> acceptedTargets,
           TargetManager::TargetType preferredTarget) : QObject(0),
  _documentManager(parent), _id(id), _label(tr("Tool")), _enabled(true),
  _action(new ToolActionWrapper(this)) {
  _acceptedTargets = acceptedTargets.isEmpty() ? TargetManager::allTargets()
                                               : acceptedTargets;
  if (_acceptedTargets.contains(preferredTarget)) {
    _preferredTarget = preferredTarget;
  } else {
    // correct preferred target in case it is not consistant w/ accepted targets
    QList<TargetManager::TargetType> list = acceptedTargets.toList();
    qSort(list);
    _preferredTarget = list.first();
  }
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

void Tool::trigger(TargetManager::TargetType targetType) {
  Q_UNUSED(targetType)
  // default: do nothing
  // LATER take care of last Tool triggered for F4 handling
  emit triggered();
}

void Tool::trigger() {
  trigger(preferredTarget());
}

bool Tool::triggerable(TargetManager::TargetType targetType) const {
  Q_UNUSED(targetType);
  return enabled();
}
