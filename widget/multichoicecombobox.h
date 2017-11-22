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
#ifndef MULTICHOICECOMBOBOX_H
#define MULTICHOICECOMBOBOX_H

#include "libh6ncsu_global.h"
#include <QComboBox>
#include <QList>
#include <QString>

class QStandardItemModel;
class QStyledItemDelegate;

/** Modified QComboBox for handling multichoices.
 * Does not support external model as an item source.
 */
class LIBH6NCSUSHARED_EXPORT MultichoiceComboBox : public QComboBox {
  Q_OBJECT
  Q_DISABLE_COPY(MultichoiceComboBox)
  Q_PROPERTY(QStringList checkedStrings READ checkedStringsAsSortedList
             NOTIFY checkedStringsChanged)
  void addItems(const QStringList &) = delete; // TODO other
  using QComboBox::setModel; // make private
  using QComboBox::setItemDelegate; // make private
  QStandardItemModel *_model;
  QStyledItemDelegate *_delegate;
  QSet<QString> _checked;
  QString _buttonText;
  mutable QStringList _checkedList;
  mutable bool _isCheckedListDirty;

public:
  MultichoiceComboBox(QWidget *parent = 0);
  void replaceItems(const QList<QString> &items, const QSet<QString> &checked);
  void replaceItems(const QList<QString> &items);
  QSet<QString> checkedStrings() const;
  QStringList checkedStringsAsSortedList() const;
  void hidePopup();
  void setButtonText(const QString &text);

signals:
  void checkedStringsChanged(const QSet<QString> &checkedStrings);

protected:
  void paintEvent(QPaintEvent *event);
};

#endif // MULTICHOICECOMBOBOX_H
