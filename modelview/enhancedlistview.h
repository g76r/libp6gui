/* Copyright 2014-2023 Hallowyn and others.
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
#ifndef ENHANCEDLISTVIEW_H
#define ENHANCEDLISTVIEW_H

#include "libp6gui_global.h"
#include <QListView>

/** Enhanced QListView
 *
 * Additionnal properties:
 * - bool ignoreInternalDrop: ignore drop events when the drag source is this
 *   widget (default: false)
 * - bool ignoreKeyboardInput: disable QListView handling of keyboard input and
 *   let container widget handle it instead (default: false)
 */
class LIBP6GUISHARED_EXPORT EnhancedListView : public QListView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreInternalDrop READ ignoreInternalDrop
             WRITE setIgnoreInternalDrop)
  Q_PROPERTY(bool ignoreKeyboardInput READ ignoreKeyboardInput
             WRITE setIgnoreKeyboardInput)
  bool _ignoreInternalDrop;
  bool _ignoreKeyboardInput;

public:
  explicit EnhancedListView(QWidget *parent = 0);
  void dropEvent(QDropEvent *e) override;
  void keyPressEvent(QKeyEvent *event) override;
  inline bool ignoreInternalDrop() const { return _ignoreInternalDrop; }
  inline void setIgnoreInternalDrop(bool ignoreInternalDrop) {
    _ignoreInternalDrop = ignoreInternalDrop; }
  bool ignoreKeyboardInput() const { return _ignoreKeyboardInput; }
  void setIgnoreKeyboardInput(bool ignoreKeyboardInput) {
    _ignoreKeyboardInput = ignoreKeyboardInput; }
};

#endif // ENHANCEDLISTVIEW_H
