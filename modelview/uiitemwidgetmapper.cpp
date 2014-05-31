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

void UiItemWidgetMapper::itemUpdated(SharedUiItem item) {
  QString id = _item.id();
  //qDebug() << "UiItemWidgetMapper::itemUpdated" << id;
  if (!id.isEmpty() && id == item.id())
    setItem(item);
}

void UiItemWidgetMapper::itemRenamed(SharedUiItem item, QString oldName) {
  if (item.isNull()) // FIXME make the delete semantics more explicit
    clearItem();
  else {
    QString id = _item.id();
    if (!id.isEmpty() && id == oldName)
      setItem(item);
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
