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
  QHash<int,QVariant> _sectionToDefaultValue;
  QHash<QWidget*,int> _widgetToSection;

public:
  explicit UiItemWidgetMapper(QObject *parent = 0);
  void addMapping(QWidget *widget, int section,
                  QVariant valueWhenNull = QVariant());
  void clearMapping();
  void removeMapping(QWidget *widget);
  void removeMapping(int section);
  void setItem(SharedUiItem item);
  void clearItem() { setItem(SharedUiItem()); }
  SharedUiItem item() const { return _item; }

public slots:
  void itemUpdated(SharedUiItem item);
  void itemRenamed(SharedUiItem item, QString oldName);

private:
  inline void populate();
  inline void populate(int section);
};

#endif // UIITEMWIDGETMAPPER_H
