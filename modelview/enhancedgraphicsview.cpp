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
#include "enhancedgraphicsview.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>

EnhancedGraphicsView::EnhancedGraphicsView(QWidget *parent)
  : QGraphicsView(parent), _mouseDragScrolling(false), _mouseMoved(false) {
  setDragMode(QGraphicsView::ScrollHandDrag);
}

void EnhancedGraphicsView::fitAllInView() {
  if (scene())
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void EnhancedGraphicsView::zoomIn() {
  scale(1.2, 1.2);
}

void EnhancedGraphicsView::zoomOut() {
  scale(1.0 / 1.2, 1.0 / 1.2);
}

void EnhancedGraphicsView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    if (event->delta() > 0) {
      zoomIn();
    } else {
      zoomOut();
    }
  } else
    QGraphicsView::wheelEvent(event);
}

void EnhancedGraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::MidButton) {
    // bidirectional grab scrolling
    if (!_mouseDragScrolling) {
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
      _mouseDragScrolling = true;
    }
  }
  QGraphicsView::mousePressEvent(event);
}

void EnhancedGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  if (_mouseDragScrolling) {
    QApplication::restoreOverrideCursor();
    _mouseDragScrolling = false;
  }
  QGraphicsView::mouseReleaseEvent(event);
  _mouseMoved = false;
}

void EnhancedGraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::MidButton) {
    // bidirectional grab scrolling
    if (!_mouseMoved) {
      _mouseMoved = true;
      _lastPos = event->pos();
    } else {
      QPointF delta = event->pos() - _lastPos;
      _lastPos = event->pos();
      horizontalScrollBar()->setValue(horizontalScrollBar()->value()-delta.x());
      verticalScrollBar()->setValue(verticalScrollBar()->value()-delta.y());
    }
  }else
    QGraphicsView::mouseMoveEvent(event);
}
