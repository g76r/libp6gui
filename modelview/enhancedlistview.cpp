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
#include "enhancedlistview.h"
#include <QDropEvent>
#include <QKeyEvent>

EnhancedListView::EnhancedListView(QWidget *parent)
  : QListView(parent), _ignoreInternalDrop(false), _ignoreKeyboardInput(false) {
}

void EnhancedListView::dropEvent(QDropEvent *e) {
  if (ignoreInternalDrop() && e->source() == this) {
    e->ignore();
    if (viewport())
      viewport()->update();
  } else
    QListView::dropEvent(e);
}

void EnhancedListView::keyPressEvent(QKeyEvent *event) {
  if (_ignoreKeyboardInput)
    event->setAccepted(false);
  else
    QListView::keyPressEvent(event);
}
