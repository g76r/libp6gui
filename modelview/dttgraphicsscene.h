#ifndef DTTGRAPHICSSCENE_H
#define DTTGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "dtt/perspectivewidget.h"

/** FIXME doc
 *
 * To have target management working, this class needs that every graphics item
 * representing a SharedUiItem return its qualified id through
 * QGraphicsItem::data(SharedUiItem::QualifiedIdRole), for instance by setting
 * the data in its constructor or when a SharedUiItem is affected to it.
 */
class LIBH6NCSUSHARED_EXPORT DttGraphicsScene : public QGraphicsScene {
  Q_OBJECT
  Q_DISABLE_COPY(DttGraphicsScene)
  QPointer<PerspectiveWidget> _perspectiveWidget;
  QStringList _selectedItemsIds, _mouseoverItemsIds;

public:
  explicit DttGraphicsScene(QObject *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const {
    return _perspectiveWidget.data(); }
  QStringList selectedItemsIds() const { return _selectedItemsIds; } // FIXME must recompute from indexes since an item id can change
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
