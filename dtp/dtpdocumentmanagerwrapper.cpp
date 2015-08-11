#include "dtpdocumentmanagerwrapper.h"
#include <QtDebug>
#include "modelview/inmemoryshareduiitemdocumentmanager.h"

DtpDocumentManagerWrapper::DtpDocumentManagerWrapper(
    SharedUiItemDocumentManager *wrapped, QObject *parent)
  : DtpDocumentManager(parent), _wrapped(wrapped) {
  connect(_wrapped, &SharedUiItemDocumentManager::itemChanged,
          this, &DtpDocumentManagerWrapper::itemChanged);
}

SharedUiItem DtpDocumentManagerWrapper::createNewItem(QString idQualifier) {
  SharedUiItem newItem = _wrapped->createNewItem(idQualifier);
  auto *imsuidm =
      qobject_cast<InMemorySharedUiItemDocumentManager*>(_wrapped);
  if (imsuidm) {
    undoStack()->push(new ChangeItemCommand(this, newItem, SharedUiItem(),
                                            idQualifier));
  }
  return newItem;
}

bool DtpDocumentManagerWrapper::changeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value) {
  auto *imsuidm = qobject_cast<InMemorySharedUiItemDocumentManager*>(_wrapped);
  if (imsuidm) {
    SharedUiItem newItem;
    if (imsuidm->changeItemByUiData(oldItem, section, value, &newItem)) {
      undoStack()->push(new ChangeItemCommand(this, newItem, oldItem,
                                              oldItem.idQualifier()));
      return true;
    }
    return false;
  }
  return _wrapped->changeItemByUiData(oldItem, section, value);
}

bool DtpDocumentManagerWrapper::changeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier) {
  if (_wrapped->changeItem(newItem, oldItem, idQualifier)) {
    auto *imsuidm =
        qobject_cast<InMemorySharedUiItemDocumentManager*>(_wrapped);
    if (imsuidm) {
      undoStack()->push(new ChangeItemCommand(this, newItem, oldItem,
                                              idQualifier));
    }
    return true;
  }
  return false;
}

void DtpDocumentManagerWrapper::doChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier) {
  if (!_wrapped->changeItem(newItem, oldItem, idQualifier))
    qWarning() << "DtpDocumentManagerWrapper::doChangeItem failed"
               << newItem << oldItem << idQualifier;
}

void DtpDocumentManagerWrapper::reorderedItems(QList<SharedUiItem> items) {
  _wrapped->reorderedItems(items);
}

SharedUiItem DtpDocumentManagerWrapper::itemById(
    QString idQualifier, QString id) const {
  return _wrapped->itemById(idQualifier, id);
}

SharedUiItem DtpDocumentManagerWrapper::itemById(QString qualifiedId) const {
  return _wrapped->itemById(qualifiedId);
}

SharedUiItemList<SharedUiItem> DtpDocumentManagerWrapper
::itemsByIdQualifier(QString idQualifier) const {
  return _wrapped->itemsByIdQualifier(idQualifier);
}

DtpDocumentManagerWrapper::ChangeItemCommand::ChangeItemCommand(
    DtpDocumentManagerWrapper *dm, SharedUiItem newItem, SharedUiItem oldItem,
    QString idQualifier, QUndoCommand *parent)
  : QUndoCommand(parent), _dm(dm), _newItem(newItem), _oldItem(oldItem),
    _idQualifier(idQualifier) {
  // LATER: compose a textual description of command and call setText
}

void DtpDocumentManagerWrapper::ChangeItemCommand::redo() {
  //qDebug() << "DtpDocumentManagerWrapper::ChangeItemCommand::redo()"
  //         << _dm << _firstRedo << _newItem << _oldItem;
  if (_dm && !_firstRedo)
    _dm->doChangeItem(_newItem, _oldItem, _idQualifier);
  else
    _firstRedo = false;
}

void DtpDocumentManagerWrapper::ChangeItemCommand::undo() {
  //qDebug() << "DtpDocumentManagerWrapper::ChangeItemCommand::undo()"
  //         << _dm << _newItem << _oldItem;
  if (_dm)
    _dm->doChangeItem(_oldItem, _newItem, _idQualifier);
}
