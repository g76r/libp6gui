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
#ifndef COMBOBOXSHAREDUIITEMDELEGATE_H
#define COMBOBOXSHAREDUIITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "modelview/shareduiitemsmodel.h"
#include "libp6gui_global.h"

// TODO rename class (ComboBoxItemDelegate, no SUI dependency)
/** Item delegate providing QComboBox based on SharedUiItemsModel for one or
 * several columns.
 * @see QComboBox
 * @see SharedUiItemsModel
 */
class LIBP6GUISHARED_EXPORT ComboBoxSharedUiItemDelegate
    : public QStyledItemDelegate {
  Q_OBJECT
  Q_DISABLE_COPY(ComboBoxSharedUiItemDelegate)
  QHash<int,QAbstractItemModel*> _models;
  QHash<int,int> _modelColumns;

public:
  enum SpecialColumns { AllColumns = -2, NoColumn = -1 };

  explicit ComboBoxSharedUiItemDelegate(QObject *parent = 0)
    : QStyledItemDelegate(parent) { }
  /** Does not take ownership of model. */
  ComboBoxSharedUiItemDelegate(
      int column, QAbstractItemModel *model, int modelColumn,
      QObject *parent = 0);
  QAbstractItemModel *model(int column) const { return _models.value(column); }
  /** @return NoColumn if not set */
  int modelColumn(int column) const {
    return _modelColumns.value(column, NoColumn); }
  /** Does not take ownership of model.
   * Can be called seral time with different column numbers to set different
   * (or same) models for different columns.
   * Column number can also be AllColumns. */
  void setModel(int column, QAbstractItemModel *model, int modelColumn);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
};

#endif // COMBOBOXSHAREDUIITEMDELEGATE_H
