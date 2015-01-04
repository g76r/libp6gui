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
#include "shareduiitemseditabletablemodel.h"

SharedUiItemsEditableTableModel::SharedUiItemsEditableTableModel(
    QObject *parent)
  : SharedUiItemsTableModel(parent), _documentManager(0) {
}

bool SharedUiItemsEditableTableModel::setData(
    const QModelIndex &index, const QVariant &value, int role) {
  SharedUiItem oldItem = itemAt(index);
  //int row = index.row();
  if (role != Qt::EditRole
      || !index.isValid()
      //|| row < 0
      //|| row >= rowCount()
      || oldItem.isNull()
      || !_documentManager
      || !_documentManager->changeItemByUiData(oldItem, index.column(), value))
    return false;
  //  _items[index.row()] = newItem;
  //  emit dataChanged(index, index);
  //emit itemChanged(newItem, oldItem);
  return true;
}
