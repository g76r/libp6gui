/* Copyright 2024 Grégoire Barbier and others.
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
#include "undocommandadapter.h"

UndoCommandAdapter::~UndoCommandAdapter() {
  delete _wrapped;
}

void UndoCommandAdapter::undo() {
  _wrapped->undo();
}

void UndoCommandAdapter::redo() {
  _wrapped->redo();
}

int UndoCommandAdapter::id() const {
  return _wrapped->id();
}

bool UndoCommandAdapter::mergeWith(const QUndoCommand *other) {
  if (auto id = _wrapped->id();
      !other || id == -1 || id != other->id()) [[unlikely]] // should never happen
    return false;
  auto casted = dynamic_cast<const UndoCommandAdapter*>(other);
  if (!casted) [[unlikely]] // should never happen
    return false;
  return _wrapped->mergeWith(casted->_wrapped);
}
