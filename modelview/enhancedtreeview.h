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
#ifndef ENHANCEDTREEVIEW_H
#define ENHANCEDTREEVIEW_H

#include <QTreeView>
#include "libh6ncsu_global.h"

/** Enhanced QTreeView
 *
 * Additionnal signals:
 * - leaved(): emited whenever leave event is received
 *
 * Additionnal properties:
 * - bool ignoreKeyboardInput: disable QTreeView handling of keyboard input and
 *   let container widget handle it instead (default: false)
 * - int expandToDepthOnChange: automaticaly expand tree to given depth every
 *   time new rows appear (default: 0, negative values mean infinite), this is
 *   convenient to display data without previous action from the user
 */
class LIBH6NCSUSHARED_EXPORT EnhancedTreeView : public QTreeView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreKeyboardInput READ ignoreKeyboardInput
             WRITE setIgnoreKeyboardInput)
  Q_PROPERTY(int expandToDepthOnChange READ expandToDepthOnChange
             WRITE setExpandToDepthOnChange)
  bool _ignoreKeyboardInput;
  int _expandToDepthOnChange;

public:
  explicit EnhancedTreeView(QWidget *parent = 0);
  void leaveEvent(QEvent *event);
  void keyPressEvent(QKeyEvent *event);
  bool ignoreKeyboardInput() const { return _ignoreKeyboardInput; }
  void setIgnoreKeyboardInput(bool ignoreKeyboardInput) {
    _ignoreKeyboardInput = ignoreKeyboardInput; }
  int expandToDepthOnChange() const { return _expandToDepthOnChange; }
  void setExpandToDepthOnChange(int expandToDepthOnChange) {
    _expandToDepthOnChange = expandToDepthOnChange; }
  void setModel(QAbstractItemModel *model);

signals:
  void leaved();

private slots:
  void rowsAppeared();
};

#endif // ENHANCEDTREEVIEW_H
