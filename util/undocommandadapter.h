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
#ifndef UNDOCOMMANDADAPTER_H
#define UNDOCOMMANDADAPTER_H

#include "libp6gui_global.h"
#include "util/coreundocommand.h"
#include <QUndoCommand>

/** Adapter for using CoreUndoCommand as a QUndoCommand. */
class LIBP6GUISHARED_EXPORT UndoCommandAdapter : public QUndoCommand {
  Q_DISABLE_COPY(UndoCommandAdapter)
  CoreUndoCommand *_wrapped;

public:
  inline UndoCommandAdapter(CoreUndoCommand *wrapped) : _wrapped(wrapped) {
    setText(_wrapped->text()); }
  ~UndoCommandAdapter();
  void undo() override;
  void redo() override;
  int id() const override;
  bool mergeWith(const QUndoCommand *other) override;
};

#endif // UNDOCOMMANDADAPTER_H
