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
#ifndef COLLAPSIBLEGROUPBOX_H
#define COLLAPSIBLEGROUPBOX_H

#include "libp6gui_global.h"
#include <QGroupBox>

class LIBP6GUISHARED_EXPORT CollapsibleGroupBox : public QGroupBox {
  Q_OBJECT
  Q_DISABLE_COPY(CollapsibleGroupBox)

public:
  explicit CollapsibleGroupBox(QWidget *parent = 0);

signals:
  void collapsed();
  void expanded();

public slots:
  void collapse(bool collapsing = true) { doCollapse(collapsing); }
  void expand(bool expanding = true) { doCollapse(!expanding); }

private:
  void doCollapse(bool collapsing);
};

#endif // COLLAPSIBLEGROUPBOX_H
