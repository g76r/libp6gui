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

bool DtpDocumentManagerWrapper::prepareChangeItem(
    QUndoCommand *command, SharedUiItem newItem, SharedUiItem oldItem,
    QString idQualifier, QString *errorString) {
  if (_wrapped->changeItem(newItem, oldItem, idQualifier, errorString)) {
    new ChangeItemCommand(this, newItem, oldItem, idQualifier, command, true);
    return true;
  }
  return false;
}

void DtpDocumentManagerWrapper::commitChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier) {
  if (!_wrapped->changeItem(newItem, oldItem, idQualifier))
    qWarning() << "DtpDocumentManagerWrapper::commitChangeItem failed"
               << newItem << oldItem << idQualifier;
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

SharedUiItem DtpDocumentManagerWrapper::createNewItem(
    QString idQualifier, QString *errorString) {
  return _wrapped->createNewItem(idQualifier, errorString);
}

bool DtpDocumentManagerWrapper::changeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
    QString *errorString) {
  return _wrapped->changeItem(newItem, oldItem, idQualifier, errorString);
}

bool DtpDocumentManagerWrapper::changeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value,
    QString *errorString) {
  return _wrapped->changeItemByUiData(oldItem, section, value, errorString);
}
