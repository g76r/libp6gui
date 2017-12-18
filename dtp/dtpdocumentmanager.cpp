/* Copyright 2014-2017 Hallowyn and others.
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
#include "dtpdocumentmanager.h"
#include <QtDebug>
#include "perspectivewidget.h"
#include <QUndoStack>
#include "closeallpoppedwindowsaction.h"
#include <QMessageBox>
#include "dtpmainwindow.h"

DtpDocumentManager::DtpDocumentManager(QObject *parent)
  : SharedUiItemDocumentManager(parent),
    _targetManager(new TargetManager(this)),
    _undoStack(new QUndoStack(this)) {
  new CloseAllPoppedWindowsAction(this);
  connect(this, &DtpDocumentManager::itemChanged,
          _targetManager, &TargetManager::itemChanged);
  connect(this, &DtpDocumentManager::dataReset, [this]() {
    _targetManager->setTarget(); // reset target on data reset
  });
}

namespace {

class UndoCommandAdapter : public QUndoCommand {
  Q_DISABLE_COPY(UndoCommandAdapter)
  CoreUndoCommand *_wrapped;

public:
  UndoCommandAdapter(CoreUndoCommand *wrapped) : _wrapped(wrapped) {
    setText(_wrapped->text()); }
  ~UndoCommandAdapter() { delete _wrapped; }
  void undo() { _wrapped->undo(); }
  void redo() { _wrapped->redo(); }
};

} // unnamed namespace

SharedUiItem DtpDocumentManager::createNewItem(
    QString idQualifier, PostCreationModifier modifier, QString *errorString) {
  QString reason;
  if (!errorString)
    errorString = &reason;
  SharedUiItem newItem;
  CoreUndoCommand *command =
      internalCreateNewItem(&newItem, idQualifier, modifier, errorString);
  if (command) {
    if (_pushChangesToUndoStack)
      undoStack()->push(new UndoCommandAdapter(command));
    else {
      command->redo();
      delete command;
    }
    return newItem;
  }
  return SharedUiItem();
}

bool DtpDocumentManager::changeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value,
    QString *errorString) {
  QString reason;
  if (!errorString)
    errorString = &reason;
  CoreUndoCommand *command =
      internalChangeItemByUiData(oldItem, section, value, errorString);
  if (command) {
    if (_pushChangesToUndoStack)
      undoStack()->push(new UndoCommandAdapter(command));
    else {
      command->redo();
      delete command;
    }
    return true;
  }
  QMessageBox::warning(DtpMainWindow::instance(),
                       "Cannot change item",
                       "Cannot change item.\n"+*errorString);
  return false;
}

bool DtpDocumentManager::changeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
    QString *errorString) {
  QString reason;
  if (!errorString)
    errorString = &reason;
  if ((!newItem.isNull() && newItem.idQualifier() != idQualifier)
      || (!oldItem.isNull() && oldItem.idQualifier() != idQualifier)) {
    *errorString =
        tr("DtpDocumentManager::changeItem called with inconsistent "
           "items id qualifiers: %1, %2, %3")
        .arg(newItem.qualifiedId()).arg(oldItem.qualifiedId()).arg(idQualifier);
    qWarning().noquote() << *errorString;
    return false;
  }
  CoreUndoCommand *command =
      internalChangeItem(newItem, oldItem, idQualifier, errorString);
  //qDebug() << "DtpDocumentManager::changeItem" << command;
  if (command) {
    // commands w/o children do nothing (e.g. deleteIfExists on inexistent item)
    // and therefore must not be pushed on undo stack but are not errors
    if (command->childCount() > 0) {
      if (_pushChangesToUndoStack)
        undoStack()->push(new UndoCommandAdapter(command));
      else {
        command->redo();
        delete command;
      }
    }
    return true;
  }
  return false;
}
