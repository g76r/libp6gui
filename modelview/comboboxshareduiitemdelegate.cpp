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
#include "comboboxshareduiitemdelegate.h"
#include <QComboBox>

ComboBoxSharedUiItemDelegate::ComboBoxSharedUiItemDelegate(
    int column, QAbstractItemModel *model, int modelColumn,
    QObject *parent) : ComboBoxSharedUiItemDelegate(parent) {
  setModel(column, model, modelColumn);
}

void ComboBoxSharedUiItemDelegate::setModel(
    int column, QAbstractItemModel *model, int modelColumn) {
  if (model) {
    _models.insert(column, model);
    _modelColumns.insert(column, modelColumn);
  } else {
    _models.remove(column);
    _modelColumns.remove(column);
  }
}

QWidget *ComboBoxSharedUiItemDelegate::createEditor(
    QWidget *parent, const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
  int column = index.column();
  QAbstractItemModel *model = _models[column];
  if (!model)
    model = _models[column = AllColumns];
  if (!model)
    return QStyledItemDelegate::createEditor(parent, option, index);
  QComboBox *comboBox = new QComboBox(parent);
  comboBox->setModel(model);
  comboBox->setModelColumn(_modelColumns[column]);
  return comboBox;
}

void ComboBoxSharedUiItemDelegate::setEditorData(
    QWidget *editor, const QModelIndex &index) const {
  auto *comboBox = qobject_cast<QComboBox*>(editor);
  if (comboBox) {
    int i = comboBox->findText(index.data(Qt::EditRole).toString());
    if (i >= 0)
      comboBox->setCurrentIndex(i);
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void ComboBoxSharedUiItemDelegate::setModelData(
    QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const {
  auto *comboBox = qobject_cast<QComboBox*>(editor);
  if (comboBox) {
    model->setData(index, comboBox->currentText(), Qt::EditRole);
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

