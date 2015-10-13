/* Copyright 2015 Hallowyn and others.
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
#include "paramsetview.h"
#include <QtDebug>
#include "util/paramsetmodel.h"

void ParamSetView::deleteSelectedParams() {
  auto *paramSetModel = static_cast<ParamSetModel*>(model());
  if (!paramSetModel) {
    qWarning() << "ParamSetView::deleteSelectedParams() called without model";
    return;
  }
  QSet<int> rows;
  foreach (const QModelIndex &index, selectedIndexes())
    rows.insert(index.row());
  QList<int> sortedRows = rows.toList();
  qSort(sortedRows);
  int alreadyRemoved = 0;
  foreach (int row, sortedRows) {
    paramSetModel->removeRows(row-alreadyRemoved, 1);
    ++alreadyRemoved;
  }
}

void ParamSetView::createNewParam() {
  auto *paramSetModel = static_cast<ParamSetModel*>(model());
  if (!paramSetModel) {
    qWarning() << "ParamSetView::createNewParam() called without model";
    return;
  }
  QString key = paramSetModel->createNewParam();
  startParamEdition(key);
}

bool ParamSetView::startParamEdition(QString key) {
  auto *paramSetModel = static_cast<ParamSetModel*>(model());
  if (!paramSetModel) {
    qWarning() << "ParamSetView::startParamEdition() called without model";
    return false;
  }
  QModelIndex index = paramSetModel->indexOf(key, false);
  if (!index.isValid())
    return false;
  scrollTo(index);
  clearSelection();
  setCurrentIndex(index);
  edit(index);
  return true;
}

void ParamSetView::setModel(QAbstractItemModel *model) {
  DtpTreeView::setModel(model);
  switch(_predefinedColumns) {
  case AllColumns:
    break;
  case KeyValue:
    hideColumn(2);
    // no break
  case KeyValueScope:
    hideColumn(3);
    break;
  case Key:
    hideColumn(2);
    // no break
  case KeyScope:
    hideColumn(1);
    hideColumn(3);
    break;
  }
}
