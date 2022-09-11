/* Copyright 2014-2022 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DTPGRAPHICSSCENE_H
#define DTPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "dtp/perspectivewidget.h"

/** FIXME doc
 *
 * To have target management working, this class needs that every graphics item
 * representing a SharedUiItem return its qualified id through
 * QGraphicsItem::data(SharedUiItem::QualifiedIdRole), for instance by setting
 * the data in its constructor or when a SharedUiItem is affected to it.
 */
class LIBP6GUISHARED_EXPORT DtpGraphicsScene : public QGraphicsScene {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsScene)
  PerspectiveWidget *_perspectiveWidget;
  QStringList _selectedItemsIds, _mouseoverItemsIds;

public:
  explicit DtpGraphicsScene(QObject *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const { return _perspectiveWidget; }
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

#endif // DTPGRAPHICSSCENE_H
