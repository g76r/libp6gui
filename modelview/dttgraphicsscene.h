#ifndef DTTGRAPHICSSCENE_H
#define DTTGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "shareduigraphicsitem.h"
#include "dtt/perspectivewidget.h"

class LIBH6NCSUSHARED_EXPORT DttGraphicsScene : public QGraphicsScene {
  Q_OBJECT
  Q_DISABLE_COPY(DttGraphicsScene)
  QList<SharedUiGraphicsItem*> _sharedUiItems;
  QPointer<PerspectiveWidget> _perspectiveWidget;
  QStringList _selectedItemsIds, _mouseoverItemsIds;

public:
  explicit DttGraphicsScene(QObject *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const {
    return _perspectiveWidget.data(); }
  QList<SharedUiGraphicsItem*> sharedUiItems() const { return _sharedUiItems; }
  using QGraphicsScene::addItem;
  void addItem(SharedUiGraphicsItem *item);
  using QGraphicsScene::removeItem;
  void removeItem(SharedUiGraphicsItem *item);
  QStringList selectedItemsIds() const { return _selectedItemsIds; }
  QStringList mouseoverItemsIds() const { return _mouseoverItemsIds; }

signals:
  void selectedItemsChanged(QStringList selectedItemsIds);

private slots:
  void propagateSelectionChanged();

private:
  friend class SharedUiGraphicsItem;
  void setMouseOverItem(QStringList ids);
  void setMouseOverItem(QString id) { setMouseOverItem(QStringList(id)); }
  void clearMouseOverItem() { setMouseOverItem(QStringList()); }
};

#endif // DTTGRAPHICSSCENE_H
