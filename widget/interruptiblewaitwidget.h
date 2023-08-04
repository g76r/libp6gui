/* Copyright 2015-2023 Gregoire Barbier and others.
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
#ifndef INTERRUPTIBLEWAITWIDGET_H
#define INTERRUPTIBLEWAITWIDGET_H

#include <QWidget>
#include "libp6gui_global.h"

class QPropertyAnimation;

namespace Ui {
class InterruptibleWaitWidget;
}

/** Widget embedable in forms to control background interruptible tasks such
 * as network requests or external process execution.
 *
 * Display a message along with an endless animated progress bar and an
 * interrupt button. Animation starts on widget show and stops when it's hidden.
 * Signal interruptRequested() is emitted when interrupt button is clicked.
 */
class LIBP6GUISHARED_EXPORT InterruptibleWaitWidget : public QWidget {
  Q_OBJECT
  Ui::InterruptibleWaitWidget *ui;
  QPropertyAnimation *_animation;

public:
  explicit InterruptibleWaitWidget(QWidget *parent = 0);
  ~InterruptibleWaitWidget();
  void setMessage(QString text);

protected:
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;

signals:
  void interruptRequested();
};

#endif // INTERRUPTIBLEWAITWIDGET_H
