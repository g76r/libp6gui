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
#include "shareduiitemproxymodel.h"
#include "modelview/shareduiitemsmodel.h"

SharedUiItemProxyModel::SharedUiItemProxyModel(QObject *parent)
  : QIdentityProxyModel(parent) {
}

QVariant SharedUiItemProxyModel::data(
    const QModelIndex &index, int role) const {
  auto suiModel = qobject_cast<SharedUiItemsModel*>(sourceModel());
  if (suiModel && role == Qt::DecorationRole)
    return _icons.value(suiModel->itemAt(mapToSource(index)).qualifier())
        .value(index.column());
  return QIdentityProxyModel::data(index, role);
}

