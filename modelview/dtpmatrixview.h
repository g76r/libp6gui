/* Copyright 2015 Hallowyn and others.
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
#ifndef DTPMATRIXVIEW_H
#define DTPMATRIXVIEW_H

#include "enhancedtableview.h"
#include "dtp/perspectivewidget.h"
#include "modelview/shareduiitem.h"
#include <QPersistentModelIndex>
#include "modelview/shareduiitemsmodel.h"

class TargetManager;

/** Document-Target-Perspective enabled QTableView, with features such as target
 * management according to widget focus, selection and mouse over.
 *
 * This class is designed for having one item per table cell. If your intend is
 * to have one item per cell, rather use DtpTableView.
 *
 * @see SharedUiItemsMatrixModel
 * @see DtpTableView
 */
class LIBH6NCSUSHARED_EXPORT  DtpMatrixView : public EnhancedTableView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpMatrixView)
  Q_PROPERTY(QStringList primaryItemsIds READ selectedItemsIds
             NOTIFY selectedItemsChanged)
  Q_PROPERTY(QStringList mouseoverItemsIds READ mouseoverItemsIds)
  Q_PROPERTY(PerspectiveWidget* perspectiveWidget READ perspectiveWidget)
  PerspectiveWidget *_perspectiveWidget;
  QPersistentModelIndex _mousePosition;
  QStringList _selectedItemsIds;
  SharedUiItemsProxyModelHelper _proxyModelHelper;

public:
  explicit DtpMatrixView(QWidget *parent = 0);
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
  void headerBinded(int section, Qt::Orientation orientation,
                    SharedUiItem newItem, SharedUiItem oldItem,
                    QString newFormula);
  void cellBinded(int row, int column, SharedUiItem newItem,
                  SharedUiItem oldItem, QString newFormula,
                  int newEditableSection);
};

#endif // DTPMATRIXVIEW_H
