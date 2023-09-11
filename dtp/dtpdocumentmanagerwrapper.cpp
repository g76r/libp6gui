/* Copyright 2014-2023 Hallowyn and others.
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
#include "dtpdocumentmanagerwrapper.h"
#include "dtpmainwindow.h"
#include "log/log.h"

DtpDocumentManagerWrapper::DtpDocumentManagerWrapper(QObject *parent)
  : DtpDocumentManager(parent), _wrapped(0) {
}

DtpDocumentManagerWrapper::DtpDocumentManagerWrapper(
    SharedUiItemDocumentManager *wrapped, QObject *parent)
  : DtpDocumentManagerWrapper(parent) {
  setWrapped(wrapped);
}

void DtpDocumentManagerWrapper::setWrapped(
    SharedUiItemDocumentManager *wrapped) {
  if (_wrapped)
    disconnect(_wrapped, 0, this, 0);
  _wrapped = wrapped;
  if (_wrapped) {
    connect(_wrapped, &SharedUiItemDocumentManager::itemChanged,
            this, &DtpDocumentManagerWrapper::itemChanged);
    connect(_wrapped, &SharedUiItemDocumentManager::dataReset,
            this, &DtpDocumentManagerWrapper::dataReset);
  }
}

SharedUiItemDocumentTransaction
*DtpDocumentManagerWrapper::internalCreateNewItem(
    SharedUiItem *newItem, Utf8String qualifier,
    PostCreationModifier modifier, QString *errorString) {
  return _wrapped->internalCreateNewItem(newItem, qualifier, modifier,
                                         errorString);
}

SharedUiItemDocumentTransaction *DtpDocumentManagerWrapper::internalChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, Utf8String qualifier,
    QString *errorString) {
  return _wrapped->internalChangeItem(newItem, oldItem, qualifier,
                                      errorString);
}

SharedUiItemDocumentTransaction
*DtpDocumentManagerWrapper::internalChangeItemByUiData(
    SharedUiItem oldItem, int section, const QVariant &value,
    QString *errorString) {
  return _wrapped->internalChangeItemByUiData(oldItem, section, value,
                                              errorString);
}

bool DtpDocumentManagerWrapper::prepareChangeItem(
    SharedUiItemDocumentTransaction *transaction, const SharedUiItem &new_item,
    const SharedUiItem &old_item, const Utf8String &qualifier,
    QString *errorString) {
  // should never be called since internalXXX() methods use wrapped dm
  Log::error() << "DtpDocumentManagerWrapper::prepareChangeItem() called "
                  "instead of wrapped one: "
               << transaction << " " << new_item.qualifiedId() << " "
               << old_item.qualifiedId() << " " << qualifier;
  return _wrapped->prepareChangeItem(transaction, new_item, old_item, qualifier,
                                     errorString);
}

void DtpDocumentManagerWrapper::commitChangeItem(
    const SharedUiItem &new_item, const SharedUiItem &old_item,
    const Utf8String &qualifier) {
  // should never be called since internalXXX() methods use wrapped dm
  Log::error() << "DtpDocumentManagerWrapper::commitChangeItem() called "
                  "instead of wrapped one: "
               << new_item.qualifiedId() << " " << old_item.qualifiedId()
               << " " << qualifier;
  _wrapped->commitChangeItem(new_item, old_item, qualifier);
}

void DtpDocumentManagerWrapper::reorderItems(
    const SharedUiItemList &items) {
  _wrapped->reorderItems(items);
}

SharedUiItem DtpDocumentManagerWrapper::itemById(
    const Utf8String &qualifier, const Utf8String &id) const {
  return _wrapped->itemById(qualifier, id);
}

SharedUiItem DtpDocumentManagerWrapper::itemById(
    const Utf8String &qualified_id) const {
  return _wrapped->itemById(qualified_id);
}

SharedUiItemList DtpDocumentManagerWrapper::itemsByQualifier(
    const Utf8String &qualifier) const {
  return _wrapped->itemsByQualifier(qualifier);
}
