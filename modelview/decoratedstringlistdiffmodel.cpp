/* Copyright 2017 Hallowyn and others.
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
#include "decoratedstringlistdiffmodel.h"
#include <QBrush>
#include <QColor>
#include <QIcon>

#define LIGHT_GREEN "#d9ead5"
#define LIGHT_LILA "#dcd7e7"
#define LIGHT_ORANGE "#fde5cc"
#define ADDED_COLOR LIGHT_GREEN
#define REMOVED_COLOR LIGHT_LILA
#define MODIFIED_COLOR LIGHT_ORANGE

#define ADDED_ICON ":fa/plus.svg"
#define REMOVED_ICON ":fa/minus.svg"
#define MODIFIED_ICON ":fa/exchange.svg"
#define NOCHANGE_ICON ":fa/check.svg"

DecoratedStringListDiffModel::DecoratedStringListDiffModel(QObject *parent)
  : StringListDiffModel(parent) {
}

QVariant DecoratedStringListDiffModel::data(
    const QModelIndex &index, int role) const {
  int column = index.column();
  switch(role) {
  case Qt::BackgroundRole:
    switch (rowStatus(index.row())) {
    case Added:
      return QBrush(QColor(LIGHT_GREEN));
    case Removed:
      return QBrush(QColor(LIGHT_LILA));
    case Modified:
      return QBrush(QColor(LIGHT_ORANGE));
    case NoChange:
      ;
    }
    return QVariant();
  case Qt::DecorationRole:
    if (column == 2) {
      switch (rowStatus(index.row())) {
      case Added:
        return QIcon(ADDED_ICON);
      case Removed:
        return QIcon(REMOVED_ICON);
      case Modified:
        return QIcon(MODIFIED_ICON);
      case NoChange:
        return QIcon(NOCHANGE_ICON);
      }
    }
    return QVariant();
  }
  return StringListDiffModel::data(index, role);
}
