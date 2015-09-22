/* Copyright 2015 Hallowyn and others.
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
#include "targetkeepingmenu.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>

TargetKeepingMenu::TargetKeepingMenu(PerspectiveWidget *parent)
  : QMenu(parent) {
}

void TargetKeepingMenu::popup(const QPoint &pos, QAction *atAction) {
  if (!_documentManager) {
    auto *perspectiveWidget = qobject_cast<PerspectiveWidget*>(parent());
    if (perspectiveWidget)
      _documentManager = perspectiveWidget->documentManager();
    else
      qWarning() << "TargetKeepingMenu::popup() called with null parent";
  }
  if (_documentManager) {
    _primaryWidget = _documentManager->targetManager()->targetWidget();
    _primaryItems = _documentManager->targetManager()->targetItems();
  } else {
    _primaryWidget = 0;
    _primaryItems.clear();
  }
  QMenu::popup(pos, atAction);
}

// LATER should also override exec() in addition to popup() ?

void TargetKeepingMenu::enterEvent(QEvent *event) {
  QMenu::enterEvent(event);
  // setting the target must be done in enterEvent() rather than showEvent()
  // because leaveEvent() is called on the perspective widget *after*
  // showEvent() is called on the QMenu
  if (_documentManager) {
    _documentManager->targetManager()->setTarget(_primaryWidget, _primaryItems);
  }
}
