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
#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>
#include <QPointer>
#include "dtpaction.h"
#include "libh6ncsu_global.h"
#include "targetmanager.h"

class DtpDocumentManager;

// FIXME is it still usefull since Tool inherits from QAction ?

class LIBH6NCSUSHARED_EXPORT ToolButton : public QAbstractButton {
  Q_OBJECT
  Q_PROPERTY(QColor flashBackground
             READ flashBackground
             WRITE setFlashBackground)
private:
  QPointer<DtpDocumentManager> _documentManager;
  QPointer<DtpAction> _tool;
  bool _mouseCurrentlyOver, _currentlyTriggerable;
  QString _keyLabel;
  QColor _flashBackground;
  QPoint _mousePressPoint;
  TargetManager::TargetType _targetType;
  int _key;
  Qt::KeyboardModifiers _modifiers;

public:
  explicit ToolButton(QWidget *parent = 0, DtpDocumentManager *documentManager = 0);
  ~ToolButton();
  void setTool(QPointer<DtpAction> tool);
  void clearTool();
  QString toolTip() const;
  /** @key as in Qt::Key, if 0 remove global key
    */
  void setGlobalKey(int key = 0,
                    Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void mouseMoveEvent(QMouseEvent *e);
  void dragEnterEvent(QDragEnterEvent *e);
  void dragMoveEvent(QDragMoveEvent *e);
  void dropEvent(QDropEvent *e);
  QColor flashBackground() const { return _flashBackground; }
  void setFlashBackground(QColor color) { _flashBackground = color; update(); }
  void trigger();
  TargetManager::TargetType targetType() const { return _targetType; }
  void setTargetType(TargetManager::TargetType targetType) {
    _targetType = targetType; }
  void setDocumentManager(DtpDocumentManager *documentManager);
  QSize sizeHint() const;

private slots:
  void targetChanged(TargetManager::TargetType targetType,
                     PerspectiveWidget *perspectiveWidget,
                     QStringList itemIds);
  void toolChanged();
  void toolTriggered();
};

#endif // TOOLBUTTON_H
