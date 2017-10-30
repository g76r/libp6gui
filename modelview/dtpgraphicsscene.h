/* Copyright 2014-2017 Hallowyn and others.
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
#ifndef DTPGRAPHICSSCENE_H
#define DTPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "dtp/perspectivewidget.h"
#include "graphics/dtpgraphicsitem.h"
#include "modelview/shareduiitemlist.h"

/** FIXME doc
 *
 * To have target management working, this class needs that every graphics item
 * representing a SharedUiItem return its qualified id through
 * QGraphicsItem::data(SharedUiItem::QualifiedIdRole), for instance by setting
 * the data in its constructor or when a SharedUiItem is affected to it.
 */
class LIBH6NCSUSHARED_EXPORT DtpGraphicsScene : public QGraphicsScene {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsScene)
  PerspectiveWidget *_perspectiveWidget;
  QStringList _selectedItemsIds, _mouseoverItemsIds, _itemQualifierFilter;
  QMultiHash<QString,QPointer<DtpGraphicsItem>> _registeredItems; // qualifiedid -> graphics item
  QHash<QString,QPointer<DtpGraphicsItem>> _itemsByMainUiItem; // qualifiedid -> graphics item

public:
  explicit DtpGraphicsScene(QObject *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const { return _perspectiveWidget; }
  QStringList selectedItemsIds() const { return _selectedItemsIds; } // FIXME must recompute from indexes since an item id can change
  QStringList mouseoverItemsIds() const { return _mouseoverItemsIds; }
  /** Set which items types can be holded by the scene depending on their id
   * qualifier.
   * This is used to filter changes received by changeItem(). */
  void setItemQualifierFilter(QStringList acceptedQualifiers) {
    _itemQualifierFilter = acceptedQualifiers; }
  void setItemQualifierFilter(
      std::initializer_list<QString> acceptedQualifiers) {
    _itemQualifierFilter = QStringList(acceptedQualifiers); }
  void setItemQualifierFilter(QString acceptedQualifier) {
    _itemQualifierFilter = QStringList(acceptedQualifier); }
  void clearItemQualifierFilter() { _itemQualifierFilter.clear(); }
  QStringList itemQualifierFilter() const { return _itemQualifierFilter; }

public slots:
  /** Calls itemChanged() on every registered DtpGraphicsItem and unregister
   * those who have been deleted meanwhile.
   * Should be overriden by subclasses to create new graphics items when
   * needed. */
  virtual void itemChanged(SharedUiItem newItem, SharedUiItem oldItem,
                           QString idQualifier);

signals:
  void selectedItemsChanged(QStringList selectedItemsIds);

protected:
  /** @return items by their main (first) registered ui item */
  QHash<QString,QPointer<DtpGraphicsItem>> itemsByMainUiItem() const {
    return _itemsByMainUiItem; }

private slots:
  void propagateSelectionChanged();

private:
  friend class DtpGraphicsItem;
  /** called by DtpGraphicsItem::setUiItems() */
  void registerDtpGraphicsItem(DtpGraphicsItem *graphicsItem,
                               SharedUiItemList<> uiItems);
  /*void setMouseOverItem(QStringList ids);
  void setMouseOverItem(QString id) { setMouseOverItem(QStringList(id)); }
  void clearMouseOverItem() { setMouseOverItem(QStringList()); }*/
};

#endif // DTPGRAPHICSSCENE_H
