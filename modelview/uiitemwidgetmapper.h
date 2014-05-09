#ifndef UIITEMWIDGETMAPPER_H
#define UIITEMWIDGETMAPPER_H

#include <QObject>
#include "modelview/shareduiitem.h"
#include "libh6ncsu_global.h"

class QWidget;

/** Similar to QDataWidgetMapper for event-driven SharedUiItem display */
class LIBH6NCSUSHARED_EXPORT UiItemWidgetMapper : public QObject {
  Q_OBJECT
  SharedUiItem _item;
  QHash<int,QWidget*> _sectionToWidget;
  QHash<QWidget*,int> _widgetToSection;
  bool _mapEmptyStringRatherThanNull;

public:
  explicit UiItemWidgetMapper(QObject *parent = 0);
  explicit UiItemWidgetMapper(bool mapEmptyStringRatherThanNull,
                              QObject *parent = 0);
  void addMapping(QWidget *widget, int section);
  void clearMapping();
  void removeMapping(QWidget *widget);
  void removeMapping(int section);
  void setItem(SharedUiItem item);
  void clearItem() { setItem(SharedUiItem()); }
  SharedUiItem item() const { return _item; }
  bool mapEmptyStringRatherThanNull() const {
    return _mapEmptyStringRatherThanNull; }
  void setMapEmptyStringRatherThanNull(bool enable = true) {
    _mapEmptyStringRatherThanNull = enable; }

public slots:
  void itemUpdated(SharedUiItem item);
  void itemRenamed(SharedUiItem item, QString oldName);

private:
  inline void populate();
  inline void populate(int section);
};

#endif // UIITEMWIDGETMAPPER_H
