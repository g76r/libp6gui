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
#include "widgetutils.h"
#include <QWidget>
#include <QStackedWidget>
#include <QTabWidget>
#include <QToolBox>

void WidgetUtils::activateAndShowWindow(QWidget *widget) {
  Q_ASSERT(widget != 0);
  widget->window()->setWindowState(
        (widget->window()->windowState()
         & ~Qt::WindowMinimized) | Qt::WindowActive);
  widget->window()->raise(); // for X11
  widget->activateWindow();
}

void WidgetUtils::ensureWidgetIsVisible(QWidget *widget) {
  Q_ASSERT(widget != 0);
  for (QWidget *child = widget, *parent = widget->parentWidget(); parent;
       child = parent, parent = parent->parentWidget()) {
    parent->setHidden(false); // includes collapsed CollapsibleGroupBox
    auto *stackedWidget = qobject_cast<QStackedWidget*>(parent);
    if (stackedWidget) {
      stackedWidget->setCurrentWidget(child);
      continue;
    }
    auto *tabWidget = qobject_cast<QTabWidget*>(parent);
    if (tabWidget) {
      tabWidget->setCurrentWidget(child);
      continue;
    }
    auto *toolbox = qobject_cast<QToolBox*>(parent);
    if (toolbox) {
      toolbox->setCurrentWidget(child);
      continue;
    }
    // LATER support folded QSplitter
  }
  activateAndShowWindow(widget);
}
