/* Copyright 2015-2023 Hallowyn and others.
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
#ifndef ENHANCEDTABLEVIEW_H
#define ENHANCEDTABLEVIEW_H

#include "libp6gui_global.h"
#include <QTableView>

/** Enhanced QTableView
 *
 * Additionnal signals:
 * - leaved(): emited whenever leave event is received
 */
class LIBP6GUISHARED_EXPORT EnhancedTableView : public QTableView {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedTableView)

public:
  explicit EnhancedTableView(QWidget *parent = 0);
  void leaveEvent(QEvent *event) override;

signals:
  void leaved();
};

#endif // ENHANCEDTABLEVIEW_H
