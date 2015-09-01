#include "dtpdocumentmanagerwrapper.h"
#include <QtDebug>
#include "modelview/inmemoryshareduiitemdocumentmanager.h"
#include <QMessageBox>
#include "dtpmainwindow.h"

DtpDocumentManagerWrapper::DtpDocumentManagerWrapper(
    SharedUiItemDocumentManager *wrapped, QObject *parent)
  : DtpDocumentManager(parent), _wrapped(wrapped) {
  connect(_wrapped, &SharedUiItemDocumentManager::itemChanged,
          this, &DtpDocumentManagerWrapper::itemChanged);
}

CoreUndoCommand *DtpDocumentManagerWrapper::internalCreateNewItem(
    SharedUiItem *newItem, QString idQualifier, QString *errorString) {
  return _wrapped->internalCreateNewItem(newItem, idQualifier, errorString);
}

CoreUndoCommand *DtpDocumentManagerWrapper::internalChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
    QString *errorString) {
  return _wrapped->internalChangeItem(newItem, oldItem, idQualifier,
                                      errorString);
}

CoreUndoCommand *DtpDocumentManagerWrapper::internalChangeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value,
    QString *errorString) {
  return _wrapped->internalChangeItemByUiData(oldItem, section, value,
                                              errorString);
}

bool DtpDocumentManagerWrapper::prepareChangeItem(
    CoreUndoCommand *command, SharedUiItem newItem, SharedUiItem oldItem,
    QString idQualifier, QString *errorString) {
  Q_UNUSED(command)
  Q_UNUSED(newItem)
  Q_UNUSED(oldItem)
  Q_UNUSED(idQualifier)
  Q_UNUSED(errorString)
  // must never be called since internalChangeItem and
  // internalChangeItemByUiData are called on wrapped DM
  Q_ASSERT(false);
  return false;
}

void DtpDocumentManagerWrapper::commitChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier) {
  Q_UNUSED(newItem)
  Q_UNUSED(oldItem)
  Q_UNUSED(idQualifier)
  // must never be called since prepareChangeItem is called on wrapped DM, the
  // commands call back its commitChangeItem() not ours
  Q_ASSERT(false);
}

void DtpDocumentManagerWrapper::reorderItems(QList<SharedUiItem> items) {
  _wrapped->reorderItems(items);
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
