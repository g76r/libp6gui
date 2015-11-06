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
#ifndef ENHANCEDTABLEVIEW_H
#define ENHANCEDTABLEVIEW_H

#include <QTableView>
#include "libh6ncsu_global.h"

/** Enhanced QTableView
 *
 * Additionnal signals:
 * - leaved(): emited whenever leave event is received
 */
class LIBH6NCSUSHARED_EXPORT EnhancedTableView : public QTableView {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedTableView)

public:
  explicit EnhancedTableView(QWidget *parent = 0);
  void leaveEvent(QEvent *event);

signals:
  void leaved();
};

#endif // ENHANCEDTABLEVIEW_H
