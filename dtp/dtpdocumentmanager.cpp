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
}

bool DtpDocumentManager::changeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value,
    QString *errorString) {
  //if (oldItem.uiData(section, Qt::EditRole) == value)
  //  return true; // nothing to do
  QString reason;
  if (!errorString)
    errorString = &reason;
  bool result = SharedUiItemDocumentManager::changeItemByUiData(
        oldItem, section, value, errorString);
  if (!result)
    QMessageBox::warning(DtpMainWindow::instance(),
                         "Cannot change item",
                         "Cannot change item: "+*errorString);
  return result;
}

bool DtpDocumentManager::changeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
    QString *errorString) {
  QUndoCommand *command = new QUndoCommand;
  QString reason;
  if (!errorString)
    errorString = &reason;
  // FIXME check constraints and apply triggers
  // LATER simplify constraints processing when called through changeItemByUiData()
  if (prepareChangeItem(command, newItem, oldItem, idQualifier, errorString)) {
    undoStack()->push(command);
    return true;
  }
  delete command;
  //QMessageBox::warning(DtpMainWindow::instance(),
  //                     "Cannot change item",
  //                     "Cannot change item: "+*errorString);
  return false;
}

DtpDocumentManager::ChangeItemCommand::ChangeItemCommand(
    DtpDocumentManager *dm, SharedUiItem newItem, SharedUiItem oldItem,
    QString idQualifier, QUndoCommand *parent, bool ignoreFirstRedo)
  : QUndoCommand(parent), _dm(dm), _newItem(newItem), _oldItem(oldItem),
    _idQualifier(idQualifier), _ignoreFirstRedo(ignoreFirstRedo) {
  // LATER: compose a textual description of command and call setText
}

void DtpDocumentManager::ChangeItemCommand::redo() {
  //qDebug() << "DtpDocumentManagerWrapper::ChangeItemCommand::redo()"
  //         << _dm << _firstRedo << _newItem << _oldItem;
  if (_dm && !_ignoreFirstRedo)
    _dm->commitChangeItem(_newItem, _oldItem, _idQualifier);
  else
    _ignoreFirstRedo = false;
}

void DtpDocumentManager::ChangeItemCommand::undo() {
  //qDebug() << "DtpDocumentManagerWrapper::ChangeItemCommand::undo()"
  //         << _dm << _newItem << _oldItem;
  if (_dm)
    _dm->commitChangeItem(_oldItem, _newItem, _idQualifier);
}
