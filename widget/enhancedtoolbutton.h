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
#ifndef ENHANCEDTOOLBUTTON_H
#define ENHANCEDTOOLBUTTON_H

#include "libp6gui_global.h"
#include <QToolButton>

/** Enhanced QToolButton
 *
 * Changed default behavior:
 * - default focus policy is Qt::NoFocus instead of Qt::TabFocus
 *
 * Additionnal features:
 * - append action shortcut to tooltip by default
 * - animate button on action trigger, even if it has not been triggered through
 *   this button (e.g. if it has been triggered through keyboard shortcut)
 */
class LIBP6GUISHARED_EXPORT EnhancedToolButton : public QToolButton {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedToolButton)

public:
  enum TriggerAnimationMethod { Disabled, AlterStyleSheet, SetDown };

private:
  TriggerAnimationMethod _triggerAnimationMethod = AlterStyleSheet;

public:
  explicit EnhancedToolButton(QWidget *parent = 0);
  void setDefaultAction(QAction *action, bool setRichTooltip = true);
  /** Default: AlterStyleSheet. */
  void setTriggerAnimationMethod(TriggerAnimationMethod method) {
    _triggerAnimationMethod = method; }
  TriggerAnimationMethod triggerAnimationMethod() const {
    return _triggerAnimationMethod; }

private:
  void fakeAnimateClick();

private slots:
  void endOfFakeAnimateClick();
};

#endif // ENHANCEDTOOLBUTTON_H
