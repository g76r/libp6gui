/* Copyright 2014-2022 Hallowyn and others.
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
#ifndef DTPDOCUMENTMANAGERWRAPPER_H
#define DTPDOCUMENTMANAGERWRAPPER_H

#include "dtpdocumentmanager.h"

/** Wrap any SharedUiItemDocumentManager to give it the properties of a
 * DtpDocumentManager.
 */
class LIBP6GUISHARED_EXPORT DtpDocumentManagerWrapper
    : public DtpDocumentManager {
  Q_OBJECT
  Q_DISABLE_COPY(DtpDocumentManagerWrapper)
  SharedUiItemDocumentManager *_wrapped;

public:
  explicit DtpDocumentManagerWrapper(QObject *parent = 0);
  /** Do not take ownership of wrapped document manager, beware that wrapped
   * object must live longer that wrapper. */
  explicit DtpDocumentManagerWrapper(
      SharedUiItemDocumentManager *wrapped, QObject *parent = 0);
  SharedUiItemDocumentManager *wrapped() const { return _wrapped; }
  /** Do not take ownership of wrapped document manager, beware that wrapped
   * object must live longer that wrapper. */
  void setWrapped(SharedUiItemDocumentManager *wrapped);
  using DtpDocumentManager::itemById;
  SharedUiItem itemById(QString idQualifier, QString id) const override;
  SharedUiItem itemById(QString qualifiedId) const override;
  using DtpDocumentManager::itemsByIdQualifier;
  SharedUiItemList<SharedUiItem> itemsByIdQualifier(
      QString idQualifier) const override;
  void reorderItems(QList<SharedUiItem> items) override;
  void registerItemType(QString idQualifier, Setter setter,
                        Creator creator) = delete;
  void addForeignKey(QString sourceQualifier, int sourceSection,
                     QString referenceQualifier, int referenceSection,
                     OnChangePolicy onUpdatePolicy,
                     OnChangePolicy onDeletePolicy) = delete;
  void addChangeItemTrigger(QString idQualifier, TriggerFlags flags,
                            ChangeItemTrigger trigger) = delete;

protected:
  bool prepareChangeItem(
      SharedUiItemDocumentTransaction *transaction, SharedUiItem newItem,
      SharedUiItem oldItem, QString idQualifier, QString *errorString) override;
  void commitChangeItem(SharedUiItem newItem, SharedUiItem oldItem,
                        QString idQualifier) override;
  SharedUiItemDocumentTransaction *internalCreateNewItem(
      SharedUiItem *newItem, QString idQualifier, PostCreationModifier modifier,
      QString *errorString) override;
  SharedUiItemDocumentTransaction *internalChangeItem(
      SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
      QString *errorString) override;
  SharedUiItemDocumentTransaction *internalChangeItemByUiData(
      SharedUiItem oldItem, int section, const QVariant &value,
      QString *errorString) override;
};

#endif // DTPDOCUMENTMANAGERWRAPPER_H
