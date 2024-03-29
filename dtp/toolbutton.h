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
#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include "dtpaction.h"
#include "targetmanager.h"
#include <QToolButton>

class DtpDocumentManager;

// LATER add context menu to change action properties e.g. target type

/** Button on which DtpAction can be drag'n droped (provided they are registered
 *  as persistent with the same DM).
 */
class LIBP6GUISHARED_EXPORT ToolButton : public QToolButton {
  Q_OBJECT
  Q_PROPERTY(QColor flashBackground
             READ flashBackground
             WRITE setFlashBackground)
private:
  typedef QToolButton super;
  QPointer<DtpDocumentManager> _documentManager;
  bool _mouseCurrentlyOver;
  QColor _flashBackground;
  QPoint _mousePressPoint;

public:
  explicit ToolButton(
      QWidget *parent = 0, DtpDocumentManager *documentManager = 0);
  ~ToolButton();
  void setDefaultAction(QAction *tool);
  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void enterEvent(QEnterEvent *) override;
  void leaveEvent(QEvent*) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void dragEnterEvent(QDragEnterEvent *e) override;
  void dragMoveEvent(QDragMoveEvent *e) override;
  void dropEvent(QDropEvent *e) override;
  QColor flashBackground() const { return _flashBackground; }
  void setFlashBackground(QColor color) { _flashBackground = color; update(); }
  void setDocumentManager(DtpDocumentManager *documentManager);
  QSize sizeHint() const override;

private slots:
  void onActionTriggered();
};

#endif // TOOLBUTTON_H
