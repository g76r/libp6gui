#include "targetmanager.h"
#include "perspectivewidget.h"
#include <QtDebug>

TargetManager::TargetManager(QObject *parent) : QObject(parent) {
  qRegisterMetaType<TargetManager::TargetType>("TargetManager::TargetType");
}

void TargetManager::setTarget(TargetType targetType,
    QPointer<PerspectiveWidget> perspectiveWidget,
    QStringList itemsIds) {
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
  // TODO is it useful to notify previous target ? is previous target even useful ? since one can remember it if needed
  if (targetType == PrimaryTarget)
    emit targetChanged(PreviousPrimaryTarget,
                       _targetWidgets[PreviousPrimaryTarget],
                       _targetItems[PreviousPrimaryTarget]);
}

QSet<TargetManager::TargetType> TargetManager::targetTypes() {
  // LATER singleton
  QSet<TargetManager::TargetType> set;
  set.insert(PrimaryTarget);
  set.insert(PreviousPrimaryTarget);
  set.insert(MouseOverTarget);
  return set;
}

void TargetManager::itemChanged(SharedUiItem newItem, SharedUiItem oldItem) {
  // Update current targets when an item id changes.
  // In many cases, there is no target containing the item at the very time it
  // changes its id, because edition in generaly done through edition widgets
  // (such QLineEdit or widgets created by item delegates) that do not manage
  // targets, therefore in many cases this method/slot is useless.
  // However, it is possible (at less in theory) that an item id changes when
  // the item is targeted.
  //qDebug() << "***** TargetManager::itemChanged" << newItem << oldItem
  //         << _targetItems[PrimaryTarget];
  if (!oldItem.isNull()) { // new items cannot already be targeted
    QString oldId = oldItem.qualifiedId();
    QString newId = newItem.qualifiedId();
    if (oldId != newId) { // only handle events where id changed
      foreach (TargetType targetType, _targetItems.keys()) {
        QStringList &itemsIds = _targetItems[targetType];
        if (newItem.isNull()) { // item removed
          itemsIds.removeAll(oldId);
        } else { // item renamed
          for (int i = 0; i < itemsIds.size(); ++i) {
            QString &id = itemsIds[i];
            if (id == oldId)
              id = newId;
          }
        }
      }
    }
  }
}
