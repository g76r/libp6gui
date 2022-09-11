/* Copyright 2014-2022 Hallowyn and others.
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
#ifndef PERSPECTIVESTACKEDWIDGET_H
#define PERSPECTIVESTACKEDWIDGET_H

#include <QStackedWidget>
#include "perspectivewidget.h"
#include "libp6gui_global.h"

class DtpDocumentManager;

/** This class is an helper for view design form which stacks several widgets
  * page and wants mouseover target handling being consistant over the stacked
  * widget. Nothing more. */
class LIBP6GUISHARED_EXPORT PerspectiveStackedWidget : public QStackedWidget {
  Q_OBJECT
  PerspectiveWidget *_perspectiveWidget;

public:
  explicit PerspectiveStackedWidget(PerspectiveWidget *parent);

public slots:
  void perspectiveChanged();

protected:
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;

private:
  inline void setMouseoverTarget(bool set);
};

#endif // PERSPECTIVESTACKEDWIDGET_H
