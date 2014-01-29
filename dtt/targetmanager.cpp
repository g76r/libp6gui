#include "targetmanager.h"
#include "perspectivewidget.h"
#include <QtDebug>

TargetManager::TargetManager(QObject *parent) : QObject(parent) {
  qRegisterMetaType<TargetManager::TargetType>("TargetManager::TargetType");
}

void TargetManager::setTarget(TargetType targetType,
    QPointer<PerspectiveWidget> perspectiveWidget,
    QList<QString> itemsIds) {
  switch (targetType) {
  case MouseOverTarget:
    break;
  case PrimaryTarget:
    _targetWidgets[PreviousPrimaryTarget] = _targetWidgets[PrimaryTarget];
    _targetItems[PreviousPrimaryTarget] = _targetItems[PrimaryTarget];
    break;
  default:
    qDebug() << "TargetManager::setTarget() called with an invalid targetType"
             << targetType;
    return;
  }
  _targetWidgets[targetType] = perspectiveWidget;
  _targetItems[targetType] = itemsIds;
  emit targetChanged(targetType, perspectiveWidget.data(), itemsIds);
}
