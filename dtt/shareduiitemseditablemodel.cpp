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
#include "shareduiitemseditablemodel.h"
#include "modelview/shareduiitem.h"
#include "modelview/shareduiitemsmodel.h"

SharedUiItemsEditableModel::SharedUiItemsEditableModel(QObject *parent)
  : QIdentityProxyModel(parent), _documentManager(0) {
}

#include <QtDebug>
bool SharedUiItemsEditableModel::setData(
    const QModelIndex &index, const QVariant &value, int role) {
  SharedUiItemsModel *model = qobject_cast<SharedUiItemsModel*>(sourceModel());
  qDebug() << "SharedUiItemsEditableModel::setData" << index << value << role
           << model << _documentManager;
  if (!model)
    return false;
  SharedUiItem oldItem = model->itemAt(index);
  qDebug() << "oldItem:" << oldItem.qualifiedId();
  return role == Qt::EditRole
      && index.isValid()
      && !oldItem.isNull()
      && _documentManager
      && _documentManager->changeItemByUiData(oldItem, index.column(), value);
}
