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
#ifndef ENHANCEDGRAPHICSVIEW_H
#define ENHANCEDGRAPHICSVIEW_H

#include "libp6gui_global.h"
#include <QGraphicsView>

/** Enhanced QGraphicsView
 *
 * Additional properties:
 * - autoDragMode: if true (the default) use RubberBandDrag drag mode on left
 *   button (rubber band selection) and mimics ScrollHandDrag on middle button
 *   (mouse grab scrolling)
 * - zoomOnCtrlWheel: if true (the default) zoom in and out on ctrl + wheel
 *
 * Additionnal slots:
 * - fit_all_in_view(): zoom in or out to fit the current items bounding box
 * - zoom_in(), zoom_out(): zoom in or out by 20%
 */
class LIBP6GUISHARED_EXPORT EnhancedGraphicsView : public QGraphicsView {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedGraphicsView)
  Q_PROPERTY(bool autoDragMode READ autoDragMode WRITE setAutoDragMode)
  Q_PROPERTY(bool zoomOnCtrlWheel READ zoomOnCtrlWheel WRITE setZoomOnCtrlWheel)

private:
  bool _mouseMoved = false, _autoDragMode = true, _zoomOnCtrlWheel = true;
  QPoint _lastPos;
  //QPointF _mouseOverPosition;

public:
  explicit EnhancedGraphicsView(QWidget *parent = 0);
  bool zoomOnCtrlWheel() const { return _zoomOnCtrlWheel; }
  void setZoomOnCtrlWheel(bool enabled = true) { _zoomOnCtrlWheel = enabled; }
  bool autoDragMode() const { return _autoDragMode; }
  void setAutoDragMode(bool enabled = true) { _autoDragMode = enabled; }
  // /** @return mouse position in scene coordinates, when mouse is overring,
  //  * tracking must be enabled, otherwise return QPointF() */
  // QPointF mouseOverPosition() const { return _mouseOverPosition; }

public slots:
  void fit_all_in_view();
  void zoom_in();
  void zoom_out();

protected:
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // ENHANCEDGRAPHICSVIEW_H
