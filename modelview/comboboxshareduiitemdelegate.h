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
#ifndef COMBOBOXSHAREDUIITEMDELEGATE_H
#define COMBOBOXSHAREDUIITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "modelview/shareduiitemsmodel.h"
#include "libh6ncsu_global.h"

/** Item delegate providing QComboBox based on SharedUiItemsModel for one or
 * several columns.
 * @see QComboBox
 * @see SharedUiItemsModel
 */
class LIBH6NCSUSHARED_EXPORT ComboBoxSharedUiItemDelegate
    : public QStyledItemDelegate {
  Q_OBJECT
  Q_DISABLE_COPY(ComboBoxSharedUiItemDelegate)
  QHash<int,SharedUiItemsModel*> _models;
  QHash<int,int> _modelColumns;

public:
  explicit ComboBoxSharedUiItemDelegate(QObject *parent = 0)
    : QStyledItemDelegate(parent) { }
  /** Does not take ownership of model. */
  ComboBoxSharedUiItemDelegate(
      int column, SharedUiItemsModel *model, int modelColumn,
      QObject *parent = 0);
  SharedUiItemsModel *model(int column) const { return _models.value(column); }
  /** @return -1 if not set */
  int modelColumn(int column) const { return _modelColumns.value(column, -1); }
  /** Does not take ownership of model.
   * Can be called seral time with different column numbers to set different
   * (or same) models for different columns. */
  void setModel(int column, SharedUiItemsModel *model, int modelColumn);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const;
};

#endif // COMBOBOXSHAREDUIITEMDELEGATE_H
