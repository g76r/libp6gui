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
#ifndef DTPTREEVIEW_H
#define DTPTREEVIEW_H

#include "enhancedtreeview.h"
#include "dtp/perspectivewidget.h"
#include "modelview/shareduiitem.h"
#include <QPersistentModelIndex>
#include "modelview/shareduiitemsmodel.h"

class TargetManager;

/** Document-Target-Perspective enabled TreeView, with features such as target
 * management according to widget focus, selection and mouse over. */
class LIBP6GUISHARED_EXPORT DtpTreeView : public EnhancedTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpTreeView)
  Q_PROPERTY(QStringList primaryItemsIds READ selectedItemsIds
             NOTIFY selectedItemsChanged)
  Q_PROPERTY(QStringList mouseoverItemsIds READ mouseoverItemsIds)
  Q_PROPERTY(PerspectiveWidget* perspectiveWidget READ perspectiveWidget)
  PerspectiveWidget *_perspectiveWidget;
  QPersistentModelIndex _mousePosition;
  QStringList _selectedItemsIds;
  SharedUiItemsProxyModelHelper _proxyModelHelper;

public:
  explicit DtpTreeView(QWidget *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const { return _perspectiveWidget; }
  void setModel(QAbstractItemModel *newModel);
  QStringList selectedItemsIds() const { return _selectedItemsIds; }
  //QPersistentModelIndex mousePosition() const { return _mousePosition; }
  bool startItemEdition(QString qualifiedId);

signals:
  void selectedItemsChanged(QStringList selectedItemsIds);

protected:
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected) override;
  void commitData(QWidget *editor);

private slots:
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

private:
  void itemHovered(const QModelIndex &index);
  void setMouseoverTarget(QString itemId);
  void setEmptyMouseoverTarget() { setMouseoverTarget(QString()); }
  void clearMouseoverTarget();
  QStringList mouseoverItemsIds() const;
};

#endif // DTPTREEVIEW_H
