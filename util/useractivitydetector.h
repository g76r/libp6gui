/* Copyright 2017 Hallowyn and others.
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
#ifndef USERACTIVITYDETECTOR_H
#define USERACTIVITYDETECTOR_H

#include "libh6ncsu_global.h"
#include <QObject>

class QTimer;

/** Send signals when user (in)activity is detected.
 * User activity is detected using KeyPressed and MouseMove event using an
 * event filter on QCoreApplication.
 */
class LIBH6NCSUSHARED_EXPORT UserActivityDetector : public QObject {
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
