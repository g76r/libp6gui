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
#include "enhancedtreeview.h"
#include <QKeyEvent>

EnhancedTreeView::EnhancedTreeView(QWidget *parent) : QTreeView(parent),
  _ignoreKeyboardInput(false), _expandToDepthOnChange(0) {
}

void EnhancedTreeView::leaveEvent(QEvent *event) {
  QTreeView::leaveEvent(event);
  emit leaved();
}

void EnhancedTreeView::keyPressEvent(QKeyEvent *event) {
  if (_ignoreKeyboardInput)
    event->setAccepted(false);
  else
    QTreeView::keyPressEvent(event);
}

void EnhancedTreeView::setModel(QAbstractItemModel *model) {
  QAbstractItemModel *old = this->model();
  if (old) {
    disconnect(model, SIGNAL(modelReset()),
               this, SLOT(rowsAppeared()));
    disconnect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
               this, SLOT(rowsAppeared()));
    disconnect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
               this, SLOT(rowsAppeared()));
  }
  QTreeView::setModel(model);
  if (model) {
    connect(model, SIGNAL(modelReset()),
            this, SLOT(rowsAppeared()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(rowsAppeared()));
    connect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(rowsAppeared()));
    rowsAppeared();
  }
}

void EnhancedTreeView::rowsAppeared() {
  if (_expandToDepthOnChange >= 0)
    expandToDepth(_expandToDepthOnChange);
  else
    expandAll();
}
