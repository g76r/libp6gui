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
#ifndef ENHANCEDLISTVIEW_H
#define ENHANCEDLISTVIEW_H

#include <QListView>
#include "libh6ncsu_global.h"

/** Enhanced QListView
 *
 * Additionnal properties:
 * - bool ignoreInternalDrop: ignore drop events when the drag source is this
 *   widget (default: false)
 * - bool ignoreKeyboardInput: disable QListView handling of keyboard input and
 *   let container widget handle it instead (default: false)
 */
class LIBH6NCSUSHARED_EXPORT EnhancedListView : public QListView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreInternalDrop READ ignoreInternalDrop
             WRITE setIgnoreInternalDrop)
  Q_PROPERTY(bool ignoreKeyboardInput READ ignoreKeyboardInput
             WRITE setIgnoreKeyboardInput)
  bool _ignoreInternalDrop;
  bool _ignoreKeyboardInput;

public:
  explicit EnhancedListView(QWidget *parent = 0);
  void dropEvent(QDropEvent *e);
  void keyPressEvent(QKeyEvent *event);
  inline bool ignoreInternalDrop() const { return _ignoreInternalDrop; }
  inline void setIgnoreInternalDrop(bool ignoreInternalDrop) {
    _ignoreInternalDrop = ignoreInternalDrop; }
  bool ignoreKeyboardInput() const { return _ignoreKeyboardInput; }
  void setIgnoreKeyboardInput(bool ignoreKeyboardInput) {
    _ignoreKeyboardInput = ignoreKeyboardInput; }
};

#endif // ENHANCEDLISTVIEW_H
