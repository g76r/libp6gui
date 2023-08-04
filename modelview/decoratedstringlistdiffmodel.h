/* Copyright 2017-2023 Hallowyn and others.
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
#ifndef DECORATEDSTRINGLISTDIFFMODEL_H
#define DECORATEDSTRINGLISTDIFFMODEL_H

#include "libp6gui_global.h"
#include "modelview/stringlistdiffmodel.h"

/** Same as StringListDiffModel, with GUI decoration (background color...)
 * @see StringListDiffModel
 */
class LIBP6GUISHARED_EXPORT DecoratedStringListDiffModel
    : public StringListDiffModel {
public:
  DecoratedStringListDiffModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif // DECORATEDSTRINGLISTDIFFMODEL_H
