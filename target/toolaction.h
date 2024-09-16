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
#ifndef TOOLACTION_H
#define TOOLACTION_H

#include "viewfinder.h"
#include <QAction>

/** ToolAction is a target-perspective aware QAction, hence it is
 *  able to change its behaviour or appearance depending on ViewFinder events.
 *
  * Constructor connects triggered() signal to on_trigger() and
  * ViewFinder::target_changed() signal to on_target_changed().
  */
class LIBP6GUISHARED_EXPORT ToolAction : public QAction {
    Q_OBJECT
    Q_DISABLE_COPY(ToolAction)
    Q_PROPERTY(ViewFinder::TargetType target_type
               READ target_type WRITE set_target_type
               NOTIFY target_type_changed)
    Q_PROPERTY(bool oneshot
               READ oneshot WRITE set_oneshot
               NOTIFY oneshot_changed)

protected:
  ViewFinder::TargetType _target_type;
  bool _oneshot;

public:
  explicit ToolAction(
      ViewFinder::TargetType target_type = ViewFinder::PrimaryTarget,
      QObject *parent = 0);
  ViewFinder::TargetType target_type() const { return _target_type; }
  void set_target_type(ViewFinder::TargetType target_type);
  bool oneshot() const { return _oneshot; }
  void set_oneshot(bool oneshot = true);

signals:
  void target_type_changed(ViewFinder::TargetType target_type);
  void oneshot_changed(bool oneshot);

protected:
  /** override this method ot perform the action, but still call it to do
   *  framework processing (such as removing oneshot tool from ViewFinder). */
  virtual void on_trigger(bool checked);
  /** overide this method if you want to react to a target change
   *  or a targetType change,
   *  common case is to setEnabled() depending on the new target */
  virtual void on_target_changed(
      ViewFinder::TargetType target_type, QWidget *perspective_widget,
      QWidget *sub_widget, const Utf8StringList &items);
};

#endif // TOOLACTION_H
