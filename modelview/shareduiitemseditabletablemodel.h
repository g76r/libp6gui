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
#ifndef SHAREDUIITEMSEDITABLETABLEMODEL_H
#define SHAREDUIITEMSEDITABLETABLEMODEL_H

#include "modelview/shareduiitemstablemodel.h"
#include "dtt/documentmanager.h"

// FIXME doc
// FIXME get all back to SharedUiItemsModel ? apart from setData() implentation ?
// FIXME make this an aspect thanks to QIdentityProxyModel ? make QronConfigGraphicalProxyModel inherit from it ?
class SharedUiItemsEditableTableModel : public SharedUiItemsTableModel {
  Q_OBJECT
  Q_DISABLE_COPY(SharedUiItemsEditableTableModel)
  DocumentManager *_documentManager;

public:
  SharedUiItemsEditableTableModel(QObject *parent = 0);
  DocumentManager *documentManager() const { return _documentManager; }
  void setDocumentManager(DocumentManager *documentManager) {
    _documentManager = documentManager; }
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
};

#endif // SHAREDUIITEMSEDITABLETABLEMODEL_H
