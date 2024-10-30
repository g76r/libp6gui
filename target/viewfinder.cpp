/* Copyright 2024 Gregoire Barbier and others.
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
#include "viewfinder.h"
#include "toolaction.h"
#include <QMessageBox>
#include <QMetaObject>
#include <QMetaMethod>
#include <QTabWidget>
#include <QApplication>

static int staticInit() {
  qRegisterMetaType<ViewFinder::TargetType>();
  return 0;
}
Q_CONSTRUCTOR_FUNCTION(staticInit)

ViewFinder *ViewFinder::_instance = 0;

ViewFinder::ViewFinder(QObject *parent) : QObject(parent) {
  auto app = qobject_cast<QApplication*>(QCoreApplication::instance());
  if (app) [[likely]] {
    connect(app, &QApplication::focusChanged,
            this, &ViewFinder::focus_changed);
  }
}

void ViewFinder::init() {
  if (_instance) [[unlikely]]
    delete _instance;
  _instance = new ViewFinder;
}

void ViewFinder::do_set_target(TargetType target_type, QWidget *widget,
                               const Utf8StringList &items) {
  Target &target = _targets[target_type];
  switch (target_type) {
    case MouseOverTarget:
      if (target._widget == widget && target._items == items) // expensive
        return; // target did not actually change
      if (target._widget || !target._items.isEmpty())
        _targets[PreviousMouseOverTarget] = target;
      break;
    case PrimaryTarget:
      if (target._widget == widget && target._items == items) // expensive
        return; // target did not actually change
      if (target._widget || !target._items.isEmpty())
        _targets[PreviousPrimaryTarget] = target;
      break;
    default:
      qDebug() << "TargetManager::set_target() called with an invalid type"
               << target_type;
      return;
  }
  target = { widget, items };
  if (target_type == PrimaryTarget) {
    const Target &prev = _targets[PreviousPrimaryTarget];
    emit target_changed(PreviousPrimaryTarget, prev._widget, prev._items);
  }
  if (target_type == MouseOverTarget) {
    const Target &prev = _targets[PreviousMouseOverTarget];
    emit target_changed(PreviousMouseOverTarget, prev._widget, prev._items);
  }
  emit target_changed(target_type, widget, items);
}

ToolAction *ViewFinder::tool(qsizetype rank) {
  return instance()->_tools.value(rank);
}

void ViewFinder::set_tool(qsizetype rank, ToolAction *tool) {
  instance()->_tools[rank] = tool;
}

void ViewFinder::do_warning(
    TargetType target_type, const Utf8String &title, const Utf8String &text) {
  QWidget *w = _targets[target_type]._widget;
  if (!w)
    w = _main_window;
  QMessageBox::warning(w, title, text | title);
}

void ViewFinder::focus_changed(QWidget *, QWidget *new_widget) {
  Utf8StringList items;
  QWidget *sub_widget = new_widget;
  //qDebug() << "widget gained focus:" << new_widget;
  while (new_widget) {
    // get primary items on deepest widget possible and keep them when found
    if (items.isEmpty()) {
      items = new_widget->property("primary_items").value<Utf8StringList>();
      if (!items.isEmpty()) {
        sub_widget = new_widget;
        break;
      }
    }
    new_widget = new_widget->parentWidget();
  }
  if (new_widget)
    set_target(sub_widget, items); // update primary target w/ or w/o items
  else
    set_target();
}
