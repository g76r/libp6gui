#include "shareduiitemwidgetmapper.h"
#include <QWidget>
#include <QMetaProperty>
#include <QtDebug>
#include "dtt/documentmanager.h"

SharedUiItemWidgetMapper::SharedUiItemWidgetMapper(QObject *parent)
  : QObject(parent), _isReadOnly(false), _documentManager(0) {
}

void SharedUiItemWidgetMapper::addMapping(QWidget *widget, int section,
                                          QVariant valueWhenNull) {
  removeMapping(section);
  removeMapping(widget);
  _sectionToWidget.insert(section, widget);
  _widgetToSection.insert(widget, section);
  _sectionToDefaultValue.insert(section, valueWhenNull);
  if (!_isReadOnly)
    connectEditionSignals(section);
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
      QVariant v = _item.uiData(section);
      if (v.isNull())
        v = _sectionToDefaultValue.value(section);
      widget->setProperty(propname, v);
    }
  }
}

#include <QLineEdit> // FIXME move
#include <QComboBox>
void SharedUiItemWidgetMapper::connectEditionSignals(int section) {
  QWidget *widget = _sectionToWidget.value(section);
  if (widget) {
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widget);
    if (lineEdit) {
      //qDebug() << "connected QLineEdit" << lineEdit << "for section" << section;
      connect(lineEdit, SIGNAL(editingFinished()),
              this, SLOT(widgetEdited()));
    } else {
      QComboBox *comboBox = qobject_cast<QComboBox*>(widget);
      if (comboBox) {
        connect(comboBox, SIGNAL(activated(int)),
                this, SLOT(widgetEdited()));
      }
    }
    // FIXME other widget types
  }
}

void SharedUiItemWidgetMapper::widgetEdited() {
  QWidget *widget = qobject_cast<QWidget*>(sender());
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
    DocumentManager *documentManager) {
  if (_documentManager) {
    disconnect(_documentManager, &DocumentManager::itemChanged,
               this, &SharedUiItemWidgetMapper::itemChanged);
  }
  _documentManager = documentManager;
  if (_documentManager) {
    connect(_documentManager, &DocumentManager::itemChanged,
            this, &SharedUiItemWidgetMapper::itemChanged);
  }
}
