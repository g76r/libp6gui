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
#include "useractivitydetector.h"
#include <QCoreApplication>
#include <QTimer>
#include <QtDebug>

UserActivityDetector::UserActivityDetector(
    int inactivityDelayMillis, QObject *parent)
  : QObject(parent), _active(true), _timer(new QTimer(this)),
    _inactivityDelay(inactivityDelayMillis) {
  _timer->setSingleShot(true);
  _timer->setInterval(inactivityDelayMillis);
  connect(_timer, &QTimer::timeout, this, &UserActivityDetector::timeout);
  _timer->start();
  auto a = QCoreApplication::instance();
  a->installEventFilter(this);
}

void UserActivityDetector::setInactivityDelay(int inactivityDelayMillis) {
  _inactivityDelay = inactivityDelayMillis;
  _timer->setInterval(inactivityDelayMillis); // what if already running ?
}

bool UserActivityDetector::eventFilter(QObject *obj, QEvent *ev) {
  if(ev->type() == QEvent::KeyPress ||
     ev->type() == QEvent::MouseMove) {
    //qDebug() << "event" << ev << ev->type();
    if (!_active) {
      _active = true;
      _timer->start();
      emit activityDetected();
    }
  }
  return QObject::eventFilter(obj, ev);
}

void UserActivityDetector::timeout() {
  if (!_active)
    return;
  _active = false;
  emit inactivityDetected();
}
