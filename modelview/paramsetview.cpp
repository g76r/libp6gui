/* Copyright 2015-2023 Hallowyn and others.
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
#include "paramsetview.h"
#include "modelview/paramsetmodel.h"

void ParamSetView::deleteSelectedParams() {
  auto *paramSetModel = static_cast<ParamSetModel*>(model());
  if (!paramSetModel) {
    qWarning() << "ParamSetView::deleteSelectedParams() called without model";
    return;
  }
  QSet<int> rows;
  for (const QModelIndex &index: selectedIndexes())
    rows.insert(index.row());
  auto sortedRows = QList<int>(rows.begin(), rows.end());
  std::sort(sortedRows.begin(), sortedRows.end());
  int alreadyRemoved = 0;
  for (int row: sortedRows) {
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
    [[fallthrough]];
  case KeyValueScope:
    hideColumn(3);
    break;
  case Key:
    hideColumn(2);
    [[fallthrough]];
  case KeyScope:
    hideColumn(1);
    hideColumn(3);
    break;
  }
}
