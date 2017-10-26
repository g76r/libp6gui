/* Copyright 2017 Hallowyn and others.
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
#ifndef DTPGRAPHICSITEM_H
#define DTPGRAPHICSITEM_H

#include <QGraphicsObject>
#include "modelview/shareduiitem.h"
#include "dtp/dtpdocumentmanager.h"

/** QGraphicsObject with DTP extensions:
 * - holding a ShareUiItem
 * - automatically getting and setting a "position" section on the SUI
 */
class LIBH6NCSUSHARED_EXPORT DtpGraphicsItem : public QGraphicsObject {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsItem)
  SharedUiItem _uiItem;
  DtpDocumentManager *_documentManager;
  int _positionSection = -1;
  QString _positionSectionName = "position";

public:
  DtpGraphicsItem(QGraphicsItem *parent = 0);
  SharedUiItem uiItem() const { return _uiItem; }
  virtual void setUiItem(SharedUiItem uiItem);
  DtpDocumentManager *documentManager() const { return _documentManager; }
  virtual void setDocumentManager(DtpDocumentManager *documentManager);
  /** SharedUiItem section used to track position, default to "position" */
  void setPositionSection(const QString &sectionName) {
    _positionSectionName = sectionName; _positionSection = -1; }
  /** SharedUiItem section used to track position, default to "position".
   * Disable the feature if section is -1 */
  void setPositionSection(int section = -1) {
    _positionSectionName = QString(); _positionSection = section; }

private:
  void persistPosition();
};

#endif // DTPGRAPHICSITEM_H
