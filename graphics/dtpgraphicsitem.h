/* Copyright 2017-2022 Hallowyn and others.
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
#ifndef DTPGRAPHICSITEM_H
#define DTPGRAPHICSITEM_H

#include <QGraphicsObject>
#include "modelview/shareduiitem.h"
#include "dtp/dtpdocumentmanager.h"

/** QGraphicsObject with DTP extensions:
 * - holding a ShareUiItem
 * - automatically getting and setting a "position" section on the SUI
 */
class LIBP6GUISHARED_EXPORT DtpGraphicsItem : public QGraphicsObject {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsItem)
  SharedUiItemList<> _uiItems;
  QString _positionSectionName = "position";

public:
  DtpGraphicsItem(QGraphicsItem *parent = 0);
  SharedUiItemList<> uiItems() const { return _uiItems; }
  virtual void setUiItems(SharedUiItemList<> uiItems = { });
  DtpDocumentManager *documentManager() const;
  /** SharedUiItem section used to track position, default to "position" */
  void setPositionSection(const QString &sectionName) {
    _positionSectionName = sectionName; }
  QVariant uiData(const QString &section, int role = Qt::DisplayRole) const;
  QVariant uiData(int section, int role = Qt::DisplayRole) const;
  QString uiString(const QString &section, int role = Qt::DisplayRole) const {
    return uiData(section, role).toString(); }
  QString uiString(int section, int role = Qt::DisplayRole) const {
    return uiData(section, role).toString(); }
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem,
                   QString idQualifier);

signals:
  /** emitted whenever at less one of the ui items changes */
  void uiItemsChanged();

private:
  void persistPosition();
};

#endif // DTPGRAPHICSITEM_H
