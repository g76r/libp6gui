/* Copyright 2014-2025 Hallowyn and others.
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
  : QGraphicsView(parent) {
  if (_autoDragMode)
    setDragMode(QGraphicsView::RubberBandDrag);
}

void EnhancedGraphicsView::fit_all_in_view() {
  if (auto s = scene(); !!s)
    fitInView(s->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void EnhancedGraphicsView::zoom_in() {
  auto anchor = transformationAnchor();
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  scale(1.2, 1.2);
  setTransformationAnchor(anchor);
}

void EnhancedGraphicsView::zoom_out() {
  auto anchor = transformationAnchor();
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  scale(1.0 / 1.2, 1.0 / 1.2);
  setTransformationAnchor(anchor);
}

void EnhancedGraphicsView::wheelEvent(QWheelEvent *event) {
  if (_zoomOnCtrlWheel && event->modifiers() & Qt::ControlModifier
      && event->angleDelta().y() != 0) {
    // ctrl + vertical wheel -> zoom
    if (event->angleDelta().y() > 0) {
      zoom_in();
    } else {
      zoom_out();
    }
    return;
  }
  QGraphicsView::wheelEvent(event);
}

void EnhancedGraphicsView::mousePressEvent(QMouseEvent *event) {
  if (_autoDragMode && event->button() == Qt::MiddleButton)
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  QGraphicsView::mousePressEvent(event);
}

void EnhancedGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  QGraphicsView::mouseReleaseEvent(event);
  if (_autoDragMode && event->button() == Qt::MiddleButton)
    QApplication::restoreOverrideCursor();
}

void EnhancedGraphicsView::mouseMoveEvent(QMouseEvent *event) {
  //_mouseOverPosition = mapToScene(event->pos());
  if (_autoDragMode && event->buttons() & Qt::MiddleButton) {
    // middle button move -> bidirectional grab scrolling
    if (!_mouseMoved) {
      _mouseMoved = true;
      _lastPos = event->pos();
    } else {
      QPointF delta = event->pos() - _lastPos;
      _lastPos = event->pos();
      horizontalScrollBar()->setValue(horizontalScrollBar()->value()-delta.x());
      verticalScrollBar()->setValue(verticalScrollBar()->value()-delta.y());
    }
    return;
  }
  QGraphicsView::mouseMoveEvent(event);
}

void EnhancedGraphicsView::leaveEvent(QEvent *event) {
  QGraphicsView::leaveEvent(event);
  //_mouseOverPosition = QPointF();
}
