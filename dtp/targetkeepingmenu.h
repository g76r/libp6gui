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
#ifndef TARGETKEEPINGMENU_H
#define TARGETKEEPINGMENU_H

#include <QMenu>
#include "perspectivewidget.h"

/** Menu that keeps previous target when shown.
 * Useful as context menu.
 * @see TargetManager
 */
class LIBH6NCSUSHARED_EXPORT TargetKeepingMenu : public QMenu {
  Q_OBJECT
  Q_DISABLE_COPY(TargetKeepingMenu)
  DtpDocumentManager *_documentManager = 0;
  PerspectiveWidget *_primaryWidget = 0;
  QStringList _primaryItems;

public:
  TargetKeepingMenu(PerspectiveWidget *parent = 0);
  void popup(const QPoint &pos, QAction *atAction = 0);
  void setDocumentManager(DtpDocumentManager *dm) { _documentManager = dm; }
  DtpDocumentManager *documentManger() const { return _documentManager; }

protected:
  void enterEvent(QEvent *event);
};

#endif // TARGETKEEPINGMENU_H
