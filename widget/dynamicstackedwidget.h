/* Copyright 2014-2023 Gregoire Barbier and others.
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
#ifndef DYNAMICSTACKEDWIDGET_H
#define DYNAMICSTACKEDWIDGET_H

#include <QStackedWidget>
#include "libp6gui_global.h"

class LIBP6GUISHARED_EXPORT DynamicStackedWidget : public QStackedWidget {
  Q_OBJECT
  // these properties can be set in QtCreator as a "dynamic property"
  Q_PROPERTY(int expandedIndex READ expandedIndex WRITE setExpandedIndex)
  Q_PROPERTY(int reducedIndex READ reducedIndex WRITE setReducedIndex)
  Q_PROPERTY(bool frozen READ frozen WRITE setFrozen)
  Q_PROPERTY(bool expandsHorizontally READ expandsHorizontally
             WRITE setExpandsHorizontally)
  // this property is only used for the purpose of animation
  Q_PROPERTY(int currentLength READ currentLength WRITE setCurrentLength)

private:
  int _expandedIndex;
  int _reducedIndex;
  bool _frozen;
  bool _shouldBeReduced;
  int _currentLength;
  bool _expandsHorizontally;

public:
  explicit DynamicStackedWidget(QWidget *parent = 0);
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void showEvent(QShowEvent *event) override;
  int expandedIndex() const { return _expandedIndex; }
  void setExpandedIndex(int expandedIndex) { _expandedIndex = expandedIndex; }
  int reducedIndex() const { return _reducedIndex; }
  void setReducedIndex(int reducedIndex) { _reducedIndex = reducedIndex; }
  bool frozen() const { return _frozen; }
  int currentLength() const { return _currentLength; }
  void setCurrentLength(int currentLength);
  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;
  bool expandsHorizontally() const { return _expandsHorizontally; }
  void setExpandsHorizontally(bool enabled);

signals:
  void reduced();
  void expanded();

public slots:
  void setFrozen(bool frozen);
  void freeze() { setFrozen(true); }
  void unfreeze() { setFrozen(false); }
  void reduce() { reduce(true); }
  void expand() { reduce(false); }
  // following slots are needed because the order of the two actions is
  // important and defining two connections would not garantee order
  void reduceAndFreeze();
  void reduceAndUnfreeze();
  void expandAndFreeze();

private:
  void reduce(bool reducing, bool shouldAnimate = true);
  Q_DISABLE_COPY(DynamicStackedWidget)
};

#endif // DYNAMICSTACKEDWIDGET_H
