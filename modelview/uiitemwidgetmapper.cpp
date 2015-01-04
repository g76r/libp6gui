#include "uiitemwidgetmapper.h"
#include <QWidget>
#include <QMetaProperty>
#include <QtDebug>

UiItemWidgetMapper::UiItemWidgetMapper(QObject *parent)
  : QObject(parent) {
}

void UiItemWidgetMapper::addMapping(QWidget *widget, int section,
                                    QVariant valueWhenNull) {
  _sectionToWidget.insert(section, widget);
  _sectionToDefaultValue.insert(section, valueWhenNull);
  populate(section);
}

void UiItemWidgetMapper::clearMapping() {
  _sectionToWidget.clear();
}

void UiItemWidgetMapper::removeMapping(QWidget *widget) {
  foreach (int i, _sectionToWidget.keys())
    if (widget == _sectionToWidget.value(i))
      _sectionToWidget.remove(i);
}

void UiItemWidgetMapper::removeMapping(int section) {
  _sectionToWidget.remove(section);
}

void UiItemWidgetMapper::setItem(SharedUiItem item) {
  _item = item;
  populate();
}

void UiItemWidgetMapper::itemChanged(SharedUiItem newItem,
                                     SharedUiItem oldItem) {
  // refresh mapping on rename or update (newItem contains new id and data)
  // stop following on delete (newItem is null)
  // do nothing on creation (oldItem is null, which cannot == non null _item)
  if (!_item.isNull() // mapper is currently following an item
      && oldItem == _item) { // related to followed item (same qualified id)
    setItem(newItem);
  }
}

void UiItemWidgetMapper::populate() {
  foreach(int i, _sectionToWidget.keys())
    populate(i);
}

void UiItemWidgetMapper::populate(int section) {
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
