#ifndef UIITEMWIDGETMAPPER_H
#define UIITEMWIDGETMAPPER_H

#include <QObject>
#include "modelview/shareduiitem.h"
#include "libh6ncsu_global.h"

class QWidget;
class DocumentManager;

/** Similar to QDataWidgetMapper for event-driven SharedUiItem display
 * @see QDataWidgetMapper
 * @see SharedUiItem */
class LIBH6NCSUSHARED_EXPORT SharedUiItemWidgetMapper : public QObject {
  Q_OBJECT
  SharedUiItem _item;
  QHash<int,QWidget*> _sectionToWidget;
  QHash<int,QVariant> _sectionToDefaultValue;
  QHash<QWidget*,int> _widgetToSection;
  bool _isReadOnly; // LATER implement read-only mode
  DocumentManager *_documentManager;

public:
  explicit SharedUiItemWidgetMapper(QObject *parent = 0);
  void addMapping(QWidget *widget, int section,
                  QVariant valueWhenNull = QVariant());
  void clearMapping();
  void removeMapping(QWidget *widget);
  void removeMapping(int section);
  void setItem(SharedUiItem item);
  void clearItem() { setItem(SharedUiItem()); }
  SharedUiItem item() const { return _item; }
  DocumentManager *documentManager() const { return _documentManager; }
  void setDocumentManager(DocumentManager *documentManager);

private slots:
  void widgetEdited();
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

private:
  void populate();
  void populate(int section);
  void connectEditionSignals(int section);
};

#endif // UIITEMWIDGETMAPPER_H
