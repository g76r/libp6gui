/* Copyright 2015 Hallowyn and others.
 * This file is part of libqtssu, see <https://github.com/g76r/libqtssu>.
 * Libqtssu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libqtssu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libqtssu.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SHAREDUIITEMSEDITABLEMODEL_H
#define SHAREDUIITEMSEDITABLEMODEL_H

#include <QIdentityProxyModel>
#include "documentmanager.h"

/** Proxy model to wrap SharedUiItemsModel subclasses and add them user
 * edition features using DocumentManager::changeItemByUiData().
 *
 * Think of this class (and of QIdentityProxyModel subclasses in general) as
 * an aspect added to a base/source SharedUiItemsModel.
 *
 * @see SharedUiItemsModel
 * @see SharedUiItemsTableModel
 * @see SharedUiItemsTreeModel
 * @see DocumentManager
 */
class LIBH6NCSUSHARED_EXPORT SharedUiItemsEditableModel
    : public QIdentityProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(SharedUiItemsEditableModel)
  DocumentManager *_documentManager;

public:
  explicit SharedUiItemsEditableModel(QObject *parent = 0);
  DocumentManager *documentManager() const { return _documentManager; }
  void setDocumentManager(DocumentManager *documentManager) {
    _documentManager = documentManager; }
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
};

#endif // SHAREDUIITEMSEDITABLEMODEL_H
