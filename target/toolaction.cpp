/* Copyright 2024 Gregoire Barbier and others.
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
#include "toolaction.h"

ToolAction::ToolAction(
    ViewFinder::TargetType target_type, QObject *parent)
  : QAction(parent), _target_type(target_type) {
  connect(this, &QAction::triggered, this, &ToolAction::on_trigger);
  connect(ViewFinder::instance(), &ViewFinder::target_changed,
          this, &ToolAction::on_target_changed);
}

void ToolAction::on_target_changed(
    ViewFinder::TargetType, QWidget *, QWidget *, const Utf8StringList &) {
}

void ToolAction::on_trigger(bool) {
  if (_oneshot)
    ViewFinder::set_tool(0, nullptr);
}

void ToolAction::set_target_type(ViewFinder::TargetType target_type) {
  if (_target_type == target_type)
    return;
  emit target_type_changed(_target_type = target_type);
  auto target = ViewFinder::target(target_type);
  on_target_changed(target_type, target._perspective_widget, target._sub_widget,
                    target._items);
}

void ToolAction::set_oneshot(bool oneshot) {
  if (_oneshot == oneshot)
    return;
  emit oneshot_changed(_oneshot = oneshot);
}
