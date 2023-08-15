/* Copyright 2017-2023 Hallowyn and others.
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
#ifndef USERACTIVITYDETECTOR_H
#define USERACTIVITYDETECTOR_H

#include "libp6gui_global.h"
#include <QObject>

class QTimer;

/** Send signals when user (in)activity is detected.
 * User activity is detected using KeyPressed and MouseMove event using an
 * event filter on QCoreApplication.
 */
class LIBP6GUISHARED_EXPORT UserActivityDetector : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(UserActivityDetector)
  bool _active;
  QTimer *_timer;
  int _inactivityDelay;

public:
  explicit UserActivityDetector(
      int inactivityDelayMillis = 60000, QObject *parent = 0);
  bool active() const { return _active; }
  int inactivityDelay() const { return _inactivityDelay; }
  void setInactivityDelay(int inactivityDelayMillis);

signals:
  void inactivityDetected();
  void activityDetected();

private:
  bool eventFilter(QObject *obj, QEvent *ev) override;
  void timeout();
};

#endif // USERACTIVITYDETECTOR_H
