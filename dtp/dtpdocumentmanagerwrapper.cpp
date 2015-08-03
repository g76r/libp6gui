#include "dtpdocumentmanagerwrapper.h"

DtpDocumentManagerWrapper::DtpDocumentManagerWrapper(
    SharedUiItemDocumentManager *wrapped, QObject *parent)
  : DtpDocumentManager(parent), _wrapped(wrapped) {
  connect(_wrapped, &SharedUiItemDocumentManager::itemChanged,
          this, &DtpDocumentManagerWrapper::itemChanged);
}

SharedUiItem DtpDocumentManagerWrapper::createNewItem(QString idQualifier) {
  return _wrapped->createNewItem(idQualifier);
}

bool DtpDocumentManagerWrapper::changeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value) {
  return _wrapped->changeItemByUiData(oldItem, section, value);
}

bool DtpDocumentManagerWrapper::changeItem(
    SharedUiItem newItem, SharedUiItem oldItem) {
  return _wrapped->changeItem(newItem, oldItem);
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

