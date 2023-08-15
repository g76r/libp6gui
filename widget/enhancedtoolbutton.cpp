/* Copyright 2014-2023 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "enhancedtoolbutton.h"
#include <QTimer>

EnhancedToolButton::EnhancedToolButton(QWidget *parent) : QToolButton(parent) {
  setFocusPolicy(Qt::NoFocus);
}

void EnhancedToolButton::setDefaultAction(
    QAction *action, bool setRichTooltip) {
  Q_ASSERT(action != 0);
  if (setRichTooltip && !action->shortcut().isEmpty())
    action->setToolTip(action->text()+" ("+action->shortcut().toString()+")");
  QToolButton::setDefaultAction(action);
  connect(action, &QAction::triggered,
          this, &EnhancedToolButton::fakeAnimateClick);
}

void EnhancedToolButton::fakeAnimateClick() {
  switch(_triggerAnimationMethod) {
  case AlterStyleSheet:
    setStyleSheet("QToolButton { background: palette(light); }");
    repaint();
    QTimer::singleShot(100, this, SLOT(endOfFakeAnimateClick()));
    break;
  case SetDown:
    if (!isDown()) {
      setDown(true);
      repaint();
      QTimer::singleShot(100, this, SLOT(endOfFakeAnimateClick()));
    }
    break;
  case Disabled:
    ;
  }
}

void EnhancedToolButton::endOfFakeAnimateClick() {
  switch(_triggerAnimationMethod) {
  case AlterStyleSheet:
    setStyleSheet("");
    repaint();
    break;
  case SetDown:
    setDown(false);
    repaint();
    break;
  case Disabled:
    ;
  }
}
