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
#include "toolbutton.h"
#include "dtpdocumentmanager.h"
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>

ToolButton::ToolButton(QWidget *parent, DtpDocumentManager *documentManager)
  : super(parent), _mouseCurrentlyOver(false), _flashBackground(Qt::lightGray) {
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setAcceptDrops(true);
  setDocumentManager(documentManager);
}

ToolButton::~ToolButton() {
}

QSize ToolButton::sizeHint() const {
  int length = (iconSize().width()+4);
  return QSize(length, length);
}

void ToolButton::setDefaultAction(QAction *action) {
  auto old = defaultAction();
  if (action == old)
    return;
  auto a = qobject_cast<DtpAction*>(old);
  if (a) {
    disconnect(a, &DtpAction::triggered,
               this, &ToolButton::onActionTriggered);
  }
  a = qobject_cast<DtpAction*>(action);
  if (a) {
    connect(a, &DtpAction::triggered,
            this, &ToolButton::onActionTriggered);
  }
  super::setDefaultAction(action);
}

void ToolButton::onActionTriggered() {
  QPropertyAnimation *a = new QPropertyAnimation(this, "flashBackground");
  a->setDuration(300);
  a->setStartValue(QColor(Qt::white));
  a->setEndValue(_flashBackground);
  a->start(QAbstractAnimation::DeleteWhenStopped);
}

void ToolButton::paintEvent(QPaintEvent *) {
  QPainter p(this);
  auto r = rect();
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::black);
  p.setBrush(_flashBackground != Qt::lightGray
      ? _flashBackground
      : _mousePressPoint.isNull()
        ? (_mouseCurrentlyOver ? Qt::darkGray : Qt::lightGray)
        : Qt::white);
  p.drawRoundedRect(r, 20, 20, Qt::RelativeSize);
  auto action = qobject_cast<DtpAction*>(defaultAction());
  if (action) {
    auto mode = action->isEnabled() ? QIcon::Normal : QIcon::Disabled;
    p.drawPixmap(2, 2, action->icon().pixmap(iconSize(), mode));
  }
  if (action) {
    QString _keyLabel = action->shortcut().toString();
    if (!_keyLabel.isNull()) {
      QPainterPath pp;
      QFont font = this->font();
      //QFont font("Sans");
      //font.setPointSize(10);
      pp.addText(QPointF(0, 0), font, _keyLabel);
      pp.translate(width()-2-pp.boundingRect().width()-pp.boundingRect().x(),
                   height()-2-pp.boundingRect().height()-pp.boundingRect().y());
      p.setRenderHint(QPainter::Antialiasing);
      // LATER parametrize outline and main letter colors
      p.setBrush(Qt::white);
      p.setPen(Qt::white);
      p.drawPath(pp);
      p.setBrush(Qt::darkBlue);
      p.setPen(Qt::transparent);
      p.drawPath(pp);
    }
    p.setPen(Qt::transparent);
    switch (action->targetType()) {
    case TargetManager::PreviousPrimaryTarget:
      p.setBrush(Qt::red);
      break;
    case TargetManager::MouseOverTarget:
      p.setBrush(Qt::blue);
      break;
    case TargetManager::PrimaryTarget:
      p.setBrush(Qt::transparent);
      break;
    }
    p.drawRect(r.right()-3,1,r.right()-1,3); // square in upper right corner
  }
  p.end();
}

void ToolButton::mousePressEvent(QMouseEvent *e) {
  QAbstractButton::mousePressEvent(e);
  _mousePressPoint = e->pos();
  update();
}

void ToolButton::mouseReleaseEvent(QMouseEvent *e) {
  _mousePressPoint = {};
  // update();
  super::mouseReleaseEvent(e);
}

void ToolButton::enterEvent(QEnterEvent *e) {
  QAbstractButton::enterEvent(e);
  _mouseCurrentlyOver = true;
  update();
}

void ToolButton::leaveEvent(QEvent *e) {
  QAbstractButton::leaveEvent(e);
  _mouseCurrentlyOver = false;
  //qDebug() << "leave";
  update();
}

void ToolButton::mouseMoveEvent(QMouseEvent *e) {
  QAbstractButton::mouseMoveEvent(e);
  // LATER another way to trigger drag is based on QApplication::startDragTime()
  auto action = qobject_cast<DtpAction*>(defaultAction());
  if (_documentManager && action && !_mousePressPoint.isNull()
      && (_mousePressPoint - e->pos()).manhattanLength()
      >= QApplication::startDragDistance()) {
    QDrag d(this);
    QMimeData *md = new QMimeData;
    md->setText(action->text());
    md->setData(MIMETYPE_ACTION_ID, action->actionId());
    d.setMimeData(md);
    QPixmap pm = action->icon().pixmap(iconSize().width());
    d.setPixmap(pm);
    d.setHotSpot(QPoint(pm.width()/2, pm.height()/2));
    Qt::DropAction da = d.exec(Qt::MoveAction | Qt::CopyAction,
                               Qt::CopyAction);
    //qDebug() << "dragged" << da << d.target() << this;
    if (da & Qt::MoveAction && d.target() != this)
      setDefaultAction(0);
    _mousePressPoint = QPoint();
    update();
  }
}

void ToolButton::dragEnterEvent(QDragEnterEvent *e) {
  if (e->mimeData()->hasFormat(MIMETYPE_ACTION_ID)) {
    e->accept();
  } else
    e->ignore();
}

void ToolButton::dragMoveEvent(QDragMoveEvent *e) {
  // by default (i.e. without modifiers) move action if possible (i.e. if from
  // another toolbutton)
  // note that if shift is pressed, at less on Windows, the event is provided
  // with move action set as default by os or Qt framework
  if ((e->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier
                         | Qt::AltModifier)) == 0) {
    if (e->possibleActions() & Qt::MoveAction)
      e->setDropAction(Qt::MoveAction);
  }
  e->accept();
}

void ToolButton::dropEvent(QDropEvent *e) {
  e->ignore();
  if (e->source() == this || !_documentManager)
    return;
  auto actionId = e->mimeData()->data(MIMETYPE_ACTION_ID);
  auto action = _documentManager->actionById(actionId);
  if (!action)
    return;
  setDefaultAction(action);
  e->accept();
  //qDebug() << "dropped" << e->possibleActions() << e->proposedAction();
}

void ToolButton::setDocumentManager(DtpDocumentManager *documentManager) {
  _documentManager = documentManager;
}
