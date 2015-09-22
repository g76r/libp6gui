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
#ifndef PERSPECTIVESTACKEDWIDGET_H
#define PERSPECTIVESTACKEDWIDGET_H

#include <QStackedWidget>
#include "perspectivewidget.h"
#include "libh6ncsu_global.h"

class DtpDocumentManager;

/** This class is an helper for view design form which stacks several widgets
  * page and wants mouseover target handling being consistant over the stacked
  * widget. Nothing more. */
class LIBH6NCSUSHARED_EXPORT PerspectiveStackedWidget : public QStackedWidget {
  Q_OBJECT
  PerspectiveWidget *_perspectiveWidget;

public:
  explicit PerspectiveStackedWidget(PerspectiveWidget *parent);
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);

public slots:
  void perspectiveChanged();

private:
  inline void setMouseoverTarget(bool set);
};

#endif // PERSPECTIVESTACKEDWIDGET_H
