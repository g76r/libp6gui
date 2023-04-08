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
#include <QtDebug>
#include "modelview/inmemoryshareduiitemdocumentmanager.h"
#include <QMessageBox>
#include "dtpmainwindow.h"

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
    SharedUiItem *newItem, QByteArray idQualifier,
    PostCreationModifier modifier, QString *errorString) {
  return _wrapped->internalCreateNewItem(newItem, idQualifier, modifier,
                                         errorString);
}

SharedUiItemDocumentTransaction *DtpDocumentManagerWrapper::internalChangeItem(
    SharedUiItem newItem, SharedUiItem oldItem, QByteArray idQualifier,
    QString *errorString) {
  return _wrapped->internalChangeItem(newItem, oldItem, idQualifier,
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
    SharedUiItemDocumentTransaction *transaction, SharedUiItem newItem,
    SharedUiItem oldItem, QByteArray idQualifier, QString *errorString) {
  Q_UNUSED(transaction)
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
    SharedUiItem newItem, SharedUiItem oldItem, QByteArray idQualifier) {
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
    QByteArray idQualifier, QByteArray id) const {
  return _wrapped->itemById(idQualifier, id);
}

SharedUiItem DtpDocumentManagerWrapper::itemById(QByteArray qualifiedId) const {
  return _wrapped->itemById(qualifiedId);
}

SharedUiItemList<SharedUiItem> DtpDocumentManagerWrapper
::itemsByIdQualifier(QByteArray idQualifier) const {
  return _wrapped->itemsByIdQualifier(idQualifier);
}
