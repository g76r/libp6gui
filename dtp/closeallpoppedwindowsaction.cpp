/* Copyright 2014-2015 Hallowyn and others.
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
#include "closeallpoppedwindowsaction.h"
#include "dtpdocumentmanager.h"
#include "perspectivewidget.h"
#include <QApplication>

CloseAllPoppedWindowsAction::CloseAllPoppedWindowsAction(QObject *parent)
  : QAction(QIcon(":fa/desktop.svg"), tr("Close All Popped Windows"), parent) {
  connect(this, &QAction::triggered, []() {
    foreach (QWidget *w, QApplication::topLevelWidgets()) {
      auto pw = qobject_cast<PerspectiveWidget*>(w);
      if (pw)
        pw->close();
    }
  });
}
