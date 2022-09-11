/* Copyright 2015-2022 Hallowyn and others.
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
#ifndef PARAMSETVIEW_H
#define PARAMSETVIEW_H

#include "dtptreeview.h"

/** View to display a ParamSet through ParamSetModel.
 * @see ParamSet
 * @see ParamSetModel
 */
class LIBP6GUISHARED_EXPORT ParamSetView : public DtpTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(ParamSetView)
public:
  enum PredefinedColumns {
    AllColumns,
    KeyValue,
    KeyValueScope,
    Key,
    KeyScope
  } _predefinedColumns;

public:
  ParamSetView(QWidget *parent = 0)
    : DtpTreeView(parent), _predefinedColumns(KeyValueScope) { }
  PredefinedColumns predefinedColumns() const { return _predefinedColumns; }
  /** Convenience attribute to hide some columns automatically on setModel().
   * Defaults to KeyValueScope. */
  void setPredefinedColumns(PredefinedColumns predefinedColumns) {
    _predefinedColumns = predefinedColumns; }
  bool startParamEdition(QString key);
  void setModel(QAbstractItemModel *model) override;

public slots:
  void deleteSelectedParams();
  void createNewParam();
};

#endif // PARAMSETVIEW_H
