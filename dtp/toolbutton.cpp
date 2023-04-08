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
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <QApplication>
#include <QPropertyAnimation>
#include <QDrag>
#include <QMimeData>
#include "dtpdocumentmanager.h"
#include <QPainterPath>

ToolButton::ToolButton(QWidget *parent, DtpDocumentManager *documentManager)
  : QAbstractButton(parent), _mouseCurrentlyOver(false),
    _flashBackground(Qt::lightGray), _targetType(TargetManager::PrimaryTarget),
    _key(0), _modifiers(Qt::NoModifier) {
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

void ToolButton::setTool(QPointer<DtpAction> tool) {
  clearTool();
  _tool = tool;
  if (!_tool.isNull()) {
    connect(_tool.data(), SIGNAL(changed()), this, SLOT(toolChanged()));
    connect(_tool.data(), SIGNAL(triggered()), this, SLOT(toolTriggered()));
    toolChanged();
  }
}

void ToolButton::clearTool() {
  if (!_tool.isNull()) {
    QObject::disconnect(_tool.data(), SIGNAL(changed()),
                        this, SLOT(toolChanged()));
    QObject::disconnect(_tool.data(), SIGNAL(triggered()),
                        this, SLOT(toolTriggered()));
  }
  _tool.clear();
}

void ToolButton::toolChanged() {
  QString toolTip = _tool ? _tool.data()->toolTip() : QString();
  if (!_keyLabel.isNull())
    toolTip.append(" (").append(_keyLabel).append(')');
  setToolTip(toolTip);
  _currentlyTriggerable = _tool ? _tool.data()->isEnabled() : false;
  update();
}

void ToolButton::toolTriggered() {
  QPropertyAnimation *a = new QPropertyAnimation(this, "flashBackground");
  a->setDuration(300);
  a->setStartValue(QColor(Qt::white));
  a->setEndValue(QColor(Qt::lightGray));
  a->start(QAbstractAnimation::DeleteWhenStopped);
}

void ToolButton::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::black);
  p.setBrush(_flashBackground != Qt::lightGray
      ? _flashBackground
      : _mousePressPoint.isNull()
        ? (_mouseCurrentlyOver ? Qt::darkGray : Qt::lightGray)
        : Qt::white);
  p.drawRoundedRect(rect(), 20, 20, Qt::RelativeSize);
  if (_tool) {
    p.drawPixmap(2, 2, _tool->icon()
                 .pixmap(iconSize(), _currentlyTriggerable
                         ? QIcon::Normal : QIcon::Disabled));
  }
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
  // LATER use icon rather than text as target indicator
  QString targetIndicator("?");
  switch (_targetType) {
  case TargetManager::PrimaryTarget:
    targetIndicator = QString();
    break;
  case TargetManager::PreviousPrimaryTarget:
    targetIndicator = "p";
    break;
  case TargetManager::MouseOverTarget:
    targetIndicator = "o";
    break;
  }
  if (!targetIndicator.isEmpty()) {
    QFont font = this->font();
    //font.setPointSize(10);
    p.setFont(font);
    p.setPen(Qt::black);
    QFontMetrics fm(font);
    p.drawText(QRectF(2, height()-2-fm.height(), width()-4, fm.maxWidth()),
               Qt::AlignLeft, targetIndicator);
  }
  p.end();
}

void ToolButton::mousePressEvent(QMouseEvent *e) {
  QAbstractButton::mousePressEvent(e);
  _mousePressPoint = e->pos();
  update();
}

void ToolButton::mouseReleaseEvent(QMouseEvent *e) {
  _mousePressPoint = QPoint();
  if (rect().contains(e->pos())) {
    switch (e->button()) {
    case Qt::LeftButton:
      QAbstractButton::mouseReleaseEvent(e); // allow click only w/ left button
      trigger();
      break;
    case Qt::RightButton:
      // LATER add a popup menu or the like rather than right-click switch
      _targetType = _targetType == TargetManager::PrimaryTarget
          ? TargetManager::MouseOverTarget : TargetManager::PrimaryTarget;
      break;
    default:
      ;
    }
    update();
  }
}

void ToolButton::trigger() {
  if (_tool)
    _tool->trigger();
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
  if (_documentManager && _tool && !_mousePressPoint.isNull()
      && (_mousePressPoint - e->pos()).manhattanLength()
      >= QApplication::startDragDistance()) {
    QDrag d(this);
    QMimeData *md = new QMimeData;
    md->setText(_tool.data()->text());
    //md->setData(MIMETYPE_TOOL_ID, _tool.data()->id().toUtf8());
    d.setMimeData(md);
    QPixmap pm = _tool.data()->icon().pixmap(iconSize().width());
    d.setPixmap(pm);
    d.setHotSpot(QPoint(pm.width()/2, pm.height()/2));
    Qt::DropAction da = d.exec(Qt::MoveAction | Qt::CopyAction,
                               Qt::CopyAction);
    //qDebug() << "dragged" << da << d.target() << this;
    if (da & Qt::MoveAction && d.target() != this)
      clearTool();
    _mousePressPoint = QPoint();
    update();
  }
}

void ToolButton::dragEnterEvent(QDragEnterEvent *e) {
  //if (e->mimeData()->hasFormat(MIMETYPE_TOOL_ID)) {
  //  e->accept();
  //} else
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
  if (e->source() == this) {
    qDebug() << "  ignored";
    e->ignore();
    return;
  }
  QPointer<DtpAction> tool;
  if (_documentManager) {
    //QString id = QString::fromUtf8(e->mimeData()->data(MIMETYPE_TOOL_ID));
    //if (!id.isEmpty())
    //  tool = _documentManager.data()->actionById(id);
  }
  if (tool.isNull()) {
    e->ignore();
    return;
  }
  setTool(tool);
  //qDebug() << "dropped" << e->possibleActions() << e->proposedAction();
  e->accept();
}

void ToolButton::targetChanged(TargetManager::TargetType targetType,
                               PerspectiveWidget *perspectiveWidget,
                               QByteArrayList itemIds) {
  Q_UNUSED(perspectiveWidget)
  Q_UNUSED(itemIds)
  //qDebug() << "ToolButton::targetChanged" << (_tool ? _tool->id() : "-") << targetType << perspectiveWidget << itemIds;
  if (targetType == _targetType && _tool && _tool->isEnabled()) {
    bool triggerable = _tool->isEnabled();
    //qDebug() << "--" << triggerable << _currentlyTriggerable;
    if (triggerable != _currentlyTriggerable)
      toolChanged();
  }
}

void ToolButton::setDocumentManager(DtpDocumentManager *documentManager) {
  if (_documentManager) {
    disconnect(_documentManager->targetManager(), &TargetManager::targetChanged,
               this, &ToolButton::targetChanged);
  }
  _documentManager = documentManager;
  if (_documentManager) {
    connect(_documentManager->targetManager(), &TargetManager::targetChanged,
            this, &ToolButton::targetChanged);
  }
}
