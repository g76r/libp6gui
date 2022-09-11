/* Copyright 2014-2022 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * Libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CLOSEALLSUBWINDOWSACTION_H
#define CLOSEALLSUBWINDOWSACTION_H

#include "libp6gui_global.h"
#include <QAction>

/** Close all PerspectiveWidget top level windows. */
class LIBP6GUISHARED_EXPORT CloseAllPoppedWindowsAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(CloseAllPoppedWindowsAction)

public:
  explicit CloseAllPoppedWindowsAction(QObject *parent);
};

#endif // CLOSEALLSUBWINDOWSACTION_H
