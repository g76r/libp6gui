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
#ifndef TARGETKEEPINGMENU_H
#define TARGETKEEPINGMENU_H

#include <QMenu>
#include "perspectivewidget.h"
#include "util/utf8string.h"

/** Menu that keeps previous target when shown.
 * Useful as context menu.
 * @see TargetManager
 */
class LIBP6GUISHARED_EXPORT TargetKeepingMenu : public QMenu {
  Q_OBJECT
  Q_DISABLE_COPY(TargetKeepingMenu)
  DtpDocumentManager *_documentManager = 0;
  PerspectiveWidget *_primaryWidget = 0;
  Utf8StringList _primaryItems;

public:
  TargetKeepingMenu(PerspectiveWidget *parent = 0);
  void popup(const QPoint &pos, QAction *atAction = 0);
  void setDocumentManager(DtpDocumentManager *dm) { _documentManager = dm; }
  DtpDocumentManager *documentManger() const { return _documentManager; }

protected:
  void enterEvent(QEnterEvent *event) override;
};

#endif // TARGETKEEPINGMENU_H
