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
#include "enhancedgraphicsview.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QScrollBar>

EnhancedGraphicsView::EnhancedGraphicsView(QWidget *parent)
  : QGraphicsView(parent), _mouseDragScrolling(false), _mouseMoved(false) {
  setDragMode(QGraphicsView::RubberBandDrag);
#if QT_VERSION >= 0x050200
  setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents); // FIXME
#endif
}

void EnhancedGraphicsView::fitAllInView() {
  if (scene())
    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void EnhancedGraphicsView::zoomIn() {
  auto anchor = transformationAnchor();
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  scale(1.2, 1.2);
  setTransformationAnchor(anchor);
}

void EnhancedGraphicsView::zoomOut() {
  auto anchor = transformationAnchor();
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  scale(1.0 / 1.2, 1.0 / 1.2);
  setTransformationAnchor(anchor);
}

void EnhancedGraphicsView::wheelEvent(QWheelEvent *event) {
  if (!(event->modifiers() & Qt::ControlModifier))
    QGraphicsView::wheelEvent(event);
  if (event->angleDelta().y() == 0)
    QGraphicsView::wheelEvent(event);
  else {
    if (event->angleDelta().y() > 0) {
      zoomIn();
    } else {
      zoomOut();
    }
  }
}

void EnhancedGraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::MiddleButton) {
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
  if (event->buttons() & Qt::MiddleButton) {
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
  } else {
    QGraphicsView::mouseMoveEvent(event);
  }
  _mouseOverPosition = mapToScene(event->pos());
}

void EnhancedGraphicsView::leaveEvent(QEvent *event) {
  QGraphicsView::leaveEvent(event);
  _mouseOverPosition = QPointF();
}
