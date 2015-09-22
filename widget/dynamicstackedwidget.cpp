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
#include "dynamicstackedwidget.h"
//#include "resizingstackedlayout.h"
#include <QtDebug>
#include <QPropertyAnimation>

DynamicStackedWidget::DynamicStackedWidget(QWidget *parent)
  : QStackedWidget(parent), _expandedIndex(0), _reducedIndex(1),
    _frozen(false), _currentLength(0) {
  setExpandsHorizontally(true);
  /*QLayout *l = layout();
  if (l)
    delete layout();
  setLayout(new ResizingStackedLayout(this));*/
}

void DynamicStackedWidget::setExpandsHorizontally(bool enabled) {
  _expandsHorizontally = enabled;
  // must have horizontal Maximum or Fixed size policy to avoid blank
  // space due to StackedWidget being expanded to larger length than its content
  // WARNING: this will be overriden by any definition in Qt Creator
  if (_expandsHorizontally)
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  else
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  // MAYDO override setSizePolicy to disable changing sizePolicy in Qt Creator
}

void DynamicStackedWidget::showEvent(QShowEvent *event) {
  QStackedWidget::showEvent(event);
  if (_frozen) {
    // reduce when showing if reduced index is current
    reduce(currentIndex() == _reducedIndex, false);
    _shouldBeReduced = true;
  } else {
    // reduce when showing
    reduce(_shouldBeReduced = true, false);
  }
}

void DynamicStackedWidget::enterEvent(QEvent *event) {
  QStackedWidget::enterEvent(event);
  _shouldBeReduced = false;
  if (!_frozen)
    reduce(false);
}

void DynamicStackedWidget::leaveEvent(QEvent *event) {
  QStackedWidget::leaveEvent(event);
  _shouldBeReduced = true;
  if (!_frozen)
    reduce(true);
}

void DynamicStackedWidget::setFrozen(bool frozen) {
  _frozen = frozen;
  if (!_frozen)
    reduce(_shouldBeReduced);
}

void DynamicStackedWidget::reduce(bool reducing, bool shouldAnimate) {
  if (shouldAnimate) {
    // TODO see if two pages can be blended by animation
    if (reducing) {
      setCurrentIndex(_reducedIndex);
      QPropertyAnimation *a = new QPropertyAnimation(this, "currentLength");
      a->setDuration(150);
      a->setStartValue(currentLength());
      if (_expandsHorizontally)
        a->setEndValue(currentWidget()->sizeHint().width());
      else
        a->setEndValue(currentWidget()->sizeHint().height());
      a->setEasingCurve(QEasingCurve::OutCubic);
      a->start(QAbstractAnimation::DeleteWhenStopped);
      emit reduced();
    } else {
      setCurrentIndex(_expandedIndex);
      QPropertyAnimation *a = new QPropertyAnimation(this, "currentLength");
      a->setDuration(150);
      a->setStartValue(currentLength());
      if (_expandsHorizontally)
        a->setEndValue(currentWidget()->sizeHint().width());
      else
        a->setEndValue(currentWidget()->sizeHint().height());
      a->setEasingCurve(QEasingCurve::OutCubic);
      a->start(QAbstractAnimation::DeleteWhenStopped);
      emit expanded();
    }
  } else {
    if (reducing) {
      setCurrentIndex(_reducedIndex);
      setCurrentLength(_expandsHorizontally
                       ? currentWidget()->sizeHint().width()
                       : currentWidget()->sizeHint().height());
      emit reduced();
    } else {
      setCurrentIndex(_expandedIndex);
      setCurrentLength(_expandsHorizontally
                       ? currentWidget()->sizeHint().width()
                       : currentWidget()->sizeHint().height());
      emit expanded();
    }
  }
}

void DynamicStackedWidget::setCurrentLength(int currentLength) {
  _currentLength = currentLength;
  updateGeometry();
}

QSize DynamicStackedWidget::sizeHint() const {
  QSize s;
  if (_expandsHorizontally)
    s = QSize(_currentLength, QFrame::sizeHint().height());
  else
    s = QSize(QFrame::sizeHint().width(), _currentLength);
  //qDebug() << "DynamicStackedWidget::sizeHint" << s << objectName();
  return s;
}

QSize DynamicStackedWidget::minimumSizeHint() const {
  QSize s = QFrame::minimumSizeHint();
  if (_expandsHorizontally) {
    if (s.width() >= 0)
      s = QSize(std::min(_currentLength, s.width()), s.height());
    else
      s = QSize(_currentLength, s.height());
  } else {
    if (s.height() >= 0)
      s = QSize(s.width(), std::min(s.height(), _currentLength));
    else
      s = QSize(s.width(), _currentLength);
  }
  //qDebug() << "DynamicStackedWidget::minimumSizeHint" << s << objectName();
  return s;
}

void DynamicStackedWidget::reduceAndFreeze() {
  setFrozen(true);
  reduce(true);
}

void DynamicStackedWidget::reduceAndUnfreeze() {
  _shouldBeReduced = true;
  setFrozen(false);
}

void DynamicStackedWidget::expandAndFreeze() {
  setFrozen(true);
  reduce(false);
}
