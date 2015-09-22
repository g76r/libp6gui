/* Copyright 2014-2015 Hallowyn and others.
 * This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
 * Libh6ncsu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libh6ncsu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "targetmanager.h"
#include "perspectivewidget.h"
#include <QtDebug>

TargetManager::TargetManager(QObject *parent) : QObject(parent) {
  qRegisterMetaType<TargetManager::TargetType>("TargetManager::TargetType");
}

void TargetManager::setTarget(TargetType targetType,
    PerspectiveWidget *perspectiveWidget,
    QStringList itemsIds) {
  switch (targetType) {
  case MouseOverTarget:
    break;
  case PrimaryTarget:
    if (_targetWidgets[PrimaryTarget]
        || !_targetItems[PrimaryTarget].isEmpty()) {
      _targetWidgets[PreviousPrimaryTarget] = _targetWidgets[PrimaryTarget];
      _targetItems[PreviousPrimaryTarget] = _targetItems[PrimaryTarget];
    }
    break;
  default:
    qDebug() << "TargetManager::setTarget() called with an invalid targetType"
             << targetType;
    return;
  }
  _targetWidgets[targetType] = perspectiveWidget;
  _targetItems[targetType] = itemsIds;
  emit targetChanged(targetType, perspectiveWidget, itemsIds);
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
  // In some cases, there is no target containing the item at the very time it
  // changes its id, because edition in done through edition widgets (such as
  // widgets created by item delegates) that do not manage targets, in this
  // cases this method/slot is called while targets are empty and does nothing.
  // However, in some other cases (for instance when edition is performed by a
  // permanent widget within an item details form that keep target set during
  // edition), this method/slot is the only way to update targets when an item
  // id changes.
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
          emit targetChanged(targetType, _targetWidgets[targetType], itemsIds);
        } else { // item renamed
          //qDebug() << "*** item renamed" << newId << oldId << itemsIds;
          for (int i = 0; i < itemsIds.size(); ++i) {
            QString &id = itemsIds[i];
            if (id == oldId) {
              //qDebug() << "*** renaming at" << i;
              id = newId;
              emit targetChanged(targetType, _targetWidgets[targetType],
                                 itemsIds);
            }
          }
          //qDebug() << "*** done" << itemsIds;
        }
      }
    }
  }
}
