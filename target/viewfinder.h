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
#ifndef VIEWFINDER_H
#define VIEWFINDER_H

#include "libp6gui_global.h"
#include "util/utf8stringlist.h"
#include <QWidget>
#include <QPointer>

class ToolAction;

/** Global (singleton) targets and tools manager.
 *
 *  For every target (SelectedTarget, HoveredTarget...) provides targeted item
 *  ids plus the widget through which they were acquired.
 *  Widgets can be null whereas items list is not empty, if items are
  * targeted outside any perspective. Widgets can be set whereas the items list
  * is empty when they are targeted with empty selection.
  * Full perspective can be deduced from targeted widget in specialized view
  * finders e.g. EntityViewFinder, they should provide a method with this
  * signature:
  *   static Perspective widget_perspective(QWidget *widget);
  *
  * Widgets used for target acquisition, or their ancestors are assumed to have
  * a Utf8StringList "selected_items" property otherwise focus follow won't
  * work.
  *
  * Tools are actions (QActions) set to be used by some GUI triggers instead of
  * a default action. By convention rank 0 tool should be used for primary
  * trigger (often mouse left click). Tools can be totaly ignored by application
  * which don't use selectable tools pattern (like a drawing app for which
  * "draw line" is a frequent tool).
  *
  * Specialized view finders can add information relative to the data model
  * or data model kind (relational, entities, etc.) for instance to perspectives
  * management.
  */
class LIBP6GUISHARED_EXPORT ViewFinder : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(ViewFinder)

public:
  enum TargetType {
    SelectedTarget, // selected: current selection, dragged items
    PreviousSelectedTarget, // previous selected, usefull for tools requesting a second target since it keeps the first one
    HoveredTarget, // can be used for mouseover (fast!) actions but also as context menu target and drop target
    PreviousHoveredTarget, // previous hovered, usefull for context menu as soon as it has shown
  };
  class Target {
  public:
    QPointer<QWidget> _widget;
    Utf8StringList _items;
  };

protected:
  QMap<TargetType,Target> _targets;
  QList<QPointer<ToolAction>> _tools;
  QWidget *_main_window;
  static ViewFinder *_instance;

public:
  explicit ViewFinder(QObject *parent = 0);
  static inline ViewFinder *instance() { return _instance; }
  /** init() must be called on specialized view finder instead of this one, if
   *  a specialized view finder is used */
  static void init();
  inline static Target target(TargetType target_type = SelectedTarget) {
    return instance()->_targets[target_type]; }
  /** Must be called each time widget changes or switch to a new
    * perspective and each time targeted items change. */
  inline static void set_target(TargetType target_type, QWidget *widget = 0,
                                const Utf8StringList &items = {}) {
    instance()->do_set_target(target_type, widget, items);
  }
  /** Syntaxic sugar. */
  inline static void set_target(
      QWidget *widget = 0, Utf8StringList items = {}) {
    instance()->do_set_target(SelectedTarget, widget, items); }
  static ToolAction *tool(qsizetype rank = 0);
  static void set_tool(qsizetype rank, ToolAction *tool);
  /** Syntaxic sugar. */
  inline static void set_tool(ToolAction *tool) {
    return set_tool(0, tool); }
  /** Warn the user using a message box centered on targeted widget */
  inline static void warning(
      TargetType target_type, const Utf8String &title,
      const Utf8String &text = {}) {
    instance()->do_warning(target_type, title, text);
  }
  inline static void set_main_window(QWidget *main_window) {
    instance()->_main_window = main_window;
  }

signals:
  /** Sent every time a target changes */
  void target_changed(
      ViewFinder::TargetType target_type, QWidget *widget,
      const Utf8StringList &items);

protected:
  virtual void focus_changed(QWidget *old_widget, QWidget *new_widget);

private:
  void do_set_target(TargetType target_type, QWidget *widget,
                     const Utf8StringList &items);
  void do_warning(
      TargetType target_type, const Utf8String &title, const Utf8String &text);
};

#endif // VIEWFINDER_H
