/* Copyright 2017-2022 Hallowyn and others.
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
#ifndef UNDOTRANSACTION_H
#define UNDOTRANSACTION_H

#include "libp6gui_global.h"
#include <QString>

class QUndoStack;
class DtpDocumentManager;
class PerspectiveWidget;

/** RAII object around QUndoStack::{begin,end}Macro().
 *
 * Beware that even though {begin,end}Macro(), and thus UndoTransaction, are
 * nestable, QUndoStack::undo() cannot be called within a macro, so a nested
 * UndoTransaction cannot rollback. So when nesting UndoTransactions, one should
 * handle rollback at toplevel (outermost transaction), by disabling auto
 * rollback on inner transactions and not rollbacking them explicitely.
 *
 * In other words: like QUndoStack macros, UndoTransaction is mainly a top level
 * UI slot object.
 */
class LIBP6GUISHARED_EXPORT UndoTransaction {
  enum State { Free, Running };
  QUndoStack *_stack = 0;
  State _state = Free;
  QString _text;
  bool _autoRollback = true;

public:
  UndoTransaction(QUndoStack *stack, const QString &text);
  UndoTransaction(DtpDocumentManager *documentManager, const QString &text);
  UndoTransaction(PerspectiveWidget *perspectiveWidget, const QString &text);
  /** Rolls back uncommitted changes. By calling endMacro() then undo(). */
  ~UndoTransaction();
  void commit();
  void rollback();
  bool autoRollback() const { return _autoRollback; }
  /** Default: true */
  void setAutoRollback(bool enable = true) { _autoRollback = enable; }
};

#endif // UNDOTRANSACTION_H
