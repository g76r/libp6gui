/* Copyright 2015-2023 Hallowyn and others.
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
#ifndef DTPTABLEVIEW_H
#define DTPTABLEVIEW_H

#include "enhancedtableview.h"
#include "dtp/perspectivewidget.h"
#include "modelview/shareduiitemsmodel.h"

class TargetManager;

// TODO test this class

/** Document-Target-Perspective enabled QTableView, with features such as target
 * management according to widget focus, selection and mouse over.
 *
 * WARNING: this class has never been tested widely and is likely to contain
 * bugs.
 *
 * This class is designed for having one item per table row. If your intend is
 * to have one item per cell, rather use DtpMatrixView.
 *
 * @see SharedUiItemsMatrixModel
 * @see DtpMatrixView
 */
class LIBP6GUISHARED_EXPORT DtpTableView : public EnhancedTableView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpTableView)
  Q_PROPERTY(QByteArrayList primaryItemsIds READ selectedItemsIds
             NOTIFY selectedItemsChanged)
  Q_PROPERTY(QByteArrayList mouseoverItemsIds READ mouseoverItemsIds)
  Q_PROPERTY(PerspectiveWidget* perspectiveWidget READ perspectiveWidget)
  PerspectiveWidget *_perspectiveWidget;
  QPersistentModelIndex _mousePosition;
  QByteArrayList _selectedItemsIds;
  SharedUiItemsProxyModelHelper _proxyModelHelper;

public:
  explicit DtpTableView(QWidget *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const { return _perspectiveWidget; }
  void setModel(QAbstractItemModel *newModel) override;
  QByteArrayList selectedItemsIds() const { return _selectedItemsIds; }
  //QPersistentModelIndex mousePosition() const { return _mousePosition; }
  bool startItemEdition(QByteArray qualifiedId);

  signals:
  void selectedItemsChanged(QByteArrayList selectedItemsIds);

  protected:
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected) override;
  void commitData(QWidget *editor) override;

  private slots:
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

  private:
  void itemHovered(const QModelIndex &index);
  void setMouseoverTarget(QByteArray itemId);
  void setEmptyMouseoverTarget() { setMouseoverTarget({}); }
  void clearMouseoverTarget();
  QByteArrayList mouseoverItemsIds() const;
};

#endif // DTPTABLEVIEW_H
