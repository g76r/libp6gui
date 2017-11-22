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
#include "multichoicecombobox.h"
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QStylePainter>
#include <QStyleOptionComboBox>
#include <QtDebug>

namespace {

class MultichoiceComboBoxItemDelegate : public QStyledItemDelegate {
public:
  MultichoiceComboBoxItemDelegate(QObject *parent = 0)
    : QStyledItemDelegate(parent) { }

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const {
    auto newOption = option;
    // ignore some decorations
    newOption.showDecorationSelected = false;
    newOption.state &= ~QStyle::State_HasFocus & ~QStyle::State_MouseOver;
    QStyledItemDelegate::paint(painter, newOption, index);
  }
};

} // unnamed namespace

MultichoiceComboBox::MultichoiceComboBox(QWidget *parent)
  : QComboBox(parent), _model(new QStandardItemModel(this)),
    _delegate(new MultichoiceComboBoxItemDelegate(this)),
    _buttonText(tr("Select Items...")), _isCheckedListDirty(true) {
  setModel(_model);
  setItemDelegate(_delegate);
  //  connect(this, &MultichoiceComboBox::checkedStringsChanged,
  //          [](QSet<QString> checkedStrings){
  //    qDebug() << "checkedStringsChanged" << checkedStrings;
  //  });
}

void MultichoiceComboBox::replaceItems(
    const QList<QString> &items, const QSet<QString> &checked) {
  _model->clear();
  int row = 0;
  for (auto string : items) {
    auto item = new QStandardItem(string);
    // must be checkable and not selectable
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(checked.contains(string) ? Qt::Checked : Qt::Unchecked,
                  Qt::CheckStateRole);
    _model->setItem(row, 0, item);
    ++row;
  }
  _isCheckedListDirty = true;
  emit checkedStringsChanged(_checked = checked);
}

void MultichoiceComboBox::replaceItems(const QList<QString> &items) {
  replaceItems(items, _checked);
}

void MultichoiceComboBox::appendItem(const QString &text, bool isChecked) {
  auto item = new QStandardItem(text);
  // must be checkable and not selectable
  item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  item->setData(isChecked ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
  _model->setItem(_model->rowCount(), 0, item);
  if (isChecked) {
    _checked.insert(text);
    _isCheckedListDirty = true;
    emit checkedStringsChanged(_checked);
  }
}

void MultichoiceComboBox::removeItem(const QString &text) {
  bool isChecked = _checked.contains(text);
  for (int row = 0; row < _model->rowCount(); ) {
    auto item = _model->item(row);
    if (item && item->text() == text) {
      _model->removeRow(row);
    } else {
      ++row;
    }
  }
  if (isChecked) {
    _checked.remove(text);
    _isCheckedListDirty = true;
    emit checkedStringsChanged(_checked);
  }
}

void MultichoiceComboBox::renameItem(
    const QString &newText, const QString &oldText) {
  bool isChecked = _checked.contains(oldText);
  for (int row = 0; row < _model->rowCount(); ++row) {
    auto item = _model->item(row);
    if (item && item->text() == oldText) {
      item->setText(newText);
    }
  }
  if (isChecked) {
    _checked.remove(oldText);
    _checked.insert(newText);
    _isCheckedListDirty = true;
    emit checkedStringsChanged(_checked);
  }
}

QSet<QString> MultichoiceComboBox::checkedStrings() const {
  return _checked;
}

QStringList MultichoiceComboBox::checkedStringsAsSortedList() const {
  if (_isCheckedListDirty) {
    QStringList list = _checked.toList();
    qSort(list);
    _checkedList = list;
    _isCheckedListDirty = false;
  }
  return _checkedList;
}

void MultichoiceComboBox::hidePopup() {
  QComboBox::hidePopup();
  QSet<QString> newChecked;
  bool changed = false;
  int n = _model->rowCount();
  for (int i = 0; i < n; ++i) {
    auto item = _model->item(i);
    QString text = item->text();
    if (item->data(Qt::CheckStateRole) == Qt::Checked) {
      newChecked.insert(text);
      if (!_checked.contains(text))
        changed = true;
    } else {
      if (_checked.contains(text))
        changed = true;
    }
  }
  if (changed) {
    _isCheckedListDirty = true;
    emit checkedStringsChanged(_checked = newChecked);
  }
}

void MultichoiceComboBox::setButtonText(const QString &text) {
  _buttonText = text;
  const int textWidth = fontMetrics().width(text);
  setMinimumWidth(textWidth + 30);
  updateGeometry();
  repaint();
}

void MultichoiceComboBox::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QStylePainter painter(this);
  painter.setPen(palette().color(QPalette::Text));
  QStyleOptionComboBox options;
  initStyleOption(&options);
  options.currentText = _buttonText;
  painter.drawComplexControl(QStyle::CC_ComboBox, options);
  painter.drawControl(QStyle::CE_ComboBoxLabel, options);
}
