/* Copyright 2014-2015 Hallowyn and others.
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
#include "shareduiitemwidgetmapper.h"
#include <QWidget>
#include <QMetaProperty>
#include <QtDebug>
#include "modelview/shareduiitemdocumentmanager.h"
#include <QLineEdit>
#include <QComboBox>
#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QAbstractButton>
#include "widget/enhancedtextedit.h"
#include "widget/enhancedplaintextedit.h"

SharedUiItemWidgetMapper::SharedUiItemWidgetMapper(QObject *parent)
  : QObject(parent), _documentManager(0) {
}

void SharedUiItemWidgetMapper::addMapping(
    QWidget *widget, int section, QVariant valueWhenNull) {
  addReadOnlyMapping(widget, section, valueWhenNull);
  connectEditionSignals(section);
}

void SharedUiItemWidgetMapper::addReadOnlyMapping(
    QWidget *widget, int section, QVariant valueWhenNull) {
  removeMapping(section);
  removeMapping(widget);
  _sectionToWidget.insert(section, widget);
  _widgetToSection.insert(widget, section);
  _sectionToDefaultValue.insert(section, valueWhenNull);
  populate(section);
}

void SharedUiItemWidgetMapper::clearMapping() {
  disconnect(0, 0, this, SLOT(widgetEdited()));
  _sectionToWidget.clear();
  _widgetToSection.clear();
}

void SharedUiItemWidgetMapper::removeMapping(QWidget *widget) {
  foreach (int section, _sectionToWidget.keys())
    if (widget == _sectionToWidget.value(section))
      _sectionToWidget.remove(section);
  disconnect(widget, 0, this, SLOT(widgetEdited()));
  _widgetToSection.remove(widget);
}

void SharedUiItemWidgetMapper::removeMapping(int section) {
  _sectionToWidget.remove(section);
  foreach (QWidget *widget, _widgetToSection.keys())
    if (section == _widgetToSection.value(widget)) {
      disconnect(widget, 0, this, SLOT(widgetEdited()));
      _widgetToSection.remove(widget);
    }
}

void SharedUiItemWidgetMapper::setItem(SharedUiItem item) {
  _item = item;
  populate();
}

void SharedUiItemWidgetMapper::itemChanged(SharedUiItem newItem,
                                           SharedUiItem oldItem) {
  // refresh mapping on rename or update (newItem contains new id and data)
  // stop following on delete (newItem is null)
  // do nothing on creation (oldItem is null, which cannot == non null _item)
  if (!_item.isNull() // mapper is currently following an item
      && oldItem == _item) { // related to followed item (same qualified id)
    setItem(newItem);
  }
}

void SharedUiItemWidgetMapper::populate() {
  foreach(int i, _sectionToWidget.keys())
    populate(i);
}

void SharedUiItemWidgetMapper::populate(int section) {
  QWidget *widget = _sectionToWidget.value(section);
  if (widget) {
    QByteArray propname = widget->metaObject()->userProperty().name();
    if (!propname.isEmpty()) {
      QVariant v = _item.uiData(section, Qt::EditRole);
      if (v.isNull())
        v = _sectionToDefaultValue.value(section);
      widget->setProperty(propname, v);
    }
  }
}

void SharedUiItemWidgetMapper::connectEditionSignals(int section) {
  QWidget *widget = _sectionToWidget.value(section);
  if (widget) {
    //qDebug() << "SharedUiItemWidgetMapper::connectEditionSignals:"
    //         << widget->metaObject()->className();
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widget);
    if (lineEdit) {
      connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(widgetEdited()));
      return;
    }
    EnhancedTextEdit *textEdit = qobject_cast<EnhancedTextEdit*>(widget);
    if (textEdit) {
      connect(textEdit, SIGNAL(editingFinished()), this, SLOT(widgetEdited()));
      return;
    }
    EnhancedPlainTextEdit *plainTextEdit
        = qobject_cast<EnhancedPlainTextEdit*>(widget);
    if (plainTextEdit) {
      connect(plainTextEdit, SIGNAL(editingFinished()),
              this, SLOT(widgetEdited()));
      return;
    }
    QComboBox *comboBox = qobject_cast<QComboBox*>(widget);
    if (comboBox) {
      connect(comboBox, SIGNAL(activated(int)), this, SLOT(widgetEdited()));
      return;
    }
    QAbstractSpinBox *spinBox = qobject_cast<QAbstractSpinBox*>(widget);
    if (spinBox) {
      connect(spinBox, SIGNAL(editingFinished()), this, SLOT(widgetEdited()));
      return;
    }
    QAbstractButton *button = qobject_cast<QAbstractButton*>(widget);
    if (button) { // includes checkable buttons, check boxes and radio buttons
      connect(button, SIGNAL(clicked(bool)), this, SLOT(widgetEdited()));
      return;
    }
    // LATER other widget types such as QCalendarWidget or maybe QGroupBox for radio/exclusive groups (anyway meanwhile one should use combo boxes)
  }
}

void SharedUiItemWidgetMapper::widgetEdited() {
  QWidget *widget = qobject_cast<QWidget*>(sender());
  //qDebug() << "SharedUiItemWidgetMapper::widgetEdited" << sender() << widget
  //         << _widgetToSection.value(widget);
  if (_documentManager && !_item.isNull() && widget
      && _widgetToSection.contains(widget)) {
    int section = _widgetToSection.value(widget);
    //qDebug() << "SharedUiItemWidgetMapper::widgetEdited" << widget << section;
    QByteArray propname = widget->metaObject()->userProperty().name();
    if (!propname.isEmpty()) {
      QVariant value = widget->property(propname);
      if (_documentManager->changeItemByUiData(_item, section, value)) {
        //qDebug() << "successfuly edited value:" << value << "item:"
        //         << _item.qualifiedId() << "section:" << section;
      } else {
        populate(section);
        //qDebug() << "rollbacked edition value:" << value << "item:"
        //         << _item.qualifiedId() << "section:" << section;
      }
    }
  } else {
    //qDebug() << "SharedUiItemWidgetMapper::widgetEdited called for nothing"
    //         << _documentManager << _item.qualifiedId() << widget;
  }
}

void SharedUiItemWidgetMapper::setDocumentManager(
    SharedUiItemDocumentManager *documentManager) {
  if (_documentManager) {
    disconnect(_documentManager, &SharedUiItemDocumentManager::itemChanged,
               this, &SharedUiItemWidgetMapper::itemChanged);
  }
  _documentManager = documentManager;
  if (_documentManager) {
    connect(_documentManager, &SharedUiItemDocumentManager::itemChanged,
            this, &SharedUiItemWidgetMapper::itemChanged);
  }
}
