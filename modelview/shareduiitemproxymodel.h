/* Copyright 2014-2015 Hallowyn and others.
 * This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
 * Libh6ncsu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libh6ncsu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SHAREDUIITEMPROXYMODEL_H
#define SHAREDUIITEMPROXYMODEL_H

#include <QIdentityProxyModel>
#include <QIcon>
#include <QString>
#include <QHash>
#include "libh6ncsu_global.h"

/** Decoration proxy model for SharedUiItemsModel.
 * @see SharedUiItemsModel */
class LIBH6NCSUSHARED_EXPORT SharedUiItemProxyModel
    : public QIdentityProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(SharedUiItemProxyModel)
  QHash<QString,QHash<int,QIcon>> _icons;

public:
  SharedUiItemProxyModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index, int role) const;
  /** Associate an id qualifier with an icon. */
  SharedUiItemProxyModel &setIcon(QString idQualifier, int column, QIcon icon) {
    _icons[idQualifier][column] = icon; return *this; }
  /** Convenience operator() for setIcon(). */
  SharedUiItemProxyModel &operator()(
      QString idQualifier, int column, QIcon icon) {
    return setIcon(idQualifier, column, icon); }
  /** Reset id qualifiers and icons associations. */
  SharedUiItemProxyModel &clearIcons() { _icons.clear(); return *this; }
};

#endif // SHAREDUIITEMPROXYMODEL_H
