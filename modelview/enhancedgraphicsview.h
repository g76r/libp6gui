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
#ifndef ENHANCEDGRAPHICSVIEW_H
#define ENHANCEDGRAPHICSVIEW_H

#include "libp6gui_global.h"
#include <QGraphicsView>

/** Enhanced QListView
 *
 * Additionnal behaviours:
 * - rubber band drag on left button by default (setDragMode(RubberBandDrag))
 * - bidirectional mouse grab scrolling on middle button
 * - zoom in and out on control + wheel up/wheel down
 *
 * Additionnal slots:
 * - fitAllInView(): zoom in or out to fit the current items bounding box
 * - zoomIn(), zoomOut(): zoom in or out by 20%
 */
class LIBP6GUISHARED_EXPORT EnhancedGraphicsView : public QGraphicsView {
  Q_OBJECT
private:
  bool _mouseDragScrolling, _mouseMoved;
  QPoint _lastPos;
  QPointF _mouseOverPosition;

public:
  explicit EnhancedGraphicsView(QWidget *parent);
  /** @return mouse position in scene coordinates, when mouse is overring,
   * tracking must be enabled, otherwise return QPointF() */
  QPointF mouseOverPosition() const { return _mouseOverPosition; }

public slots:
  void fitAllInView();
  void zoomIn();
  void zoomOut();

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // ENHANCEDGRAPHICSVIEW_H
