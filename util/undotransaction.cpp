/* Copyright 2017 Hallowyn and others.
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
#include "undotransaction.h"
#include <QUndoStack>
#include <QtDebug>
#include "dtp/dtpdocumentmanager.h"
#include "dtp/perspectivewidget.h"

UndoTransaction::UndoTransaction(QUndoStack *stack, const QString &text)
  : _stack(stack), _state(stack ? Running : Free), _text(text) {
  if (!stack) {
    qWarning() << "UndoTransaction with null undo stack";
    return;
  }
  stack->beginMacro(text);
}

UndoTransaction::UndoTransaction(
    DtpDocumentManager *documentManager, const QString &text)
  : UndoTransaction(documentManager ? documentManager->undoStack() : 0, text) {
}

UndoTransaction::UndoTransaction(
    PerspectiveWidget *perspectiveWidget, const QString &text)
  : UndoTransaction(perspectiveWidget
                    ? perspectiveWidget->documentManager() : 0, text) {
}

UndoTransaction::~UndoTransaction() {
  if (!_stack)
    return;
  _stack->endMacro();
  _stack->undo();
}

void UndoTransaction::commit() {
  if (!_stack)
    return;
  _stack->endMacro();
  _stack->beginMacro(_text);
}

void UndoTransaction::rollback() {
  if (!_stack)
    return;
  _stack->endMacro();
  _stack->undo();
  _stack->beginMacro(_text);
}
