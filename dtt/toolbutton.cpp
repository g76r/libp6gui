#include "toolbutton.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QtDebug>
#include <QApplication>
#include <QPropertyAnimation>
#include <QDrag>
#include <QMimeData>
#include "documentmanager.h"

ToolButton::ToolButton(DocumentManager *documentManager, QWidget *parent)
  : QAbstractButton(parent), _documentManager(documentManager),
    _mouseCurrentlyOver(false),
    _flashBackground(Qt::lightGray), _targetType(TargetManager::PrimaryTarget) {
  setMaximumSize(36, 36);
  setMinimumSize(36, 36);
  setAcceptDrops(true);
  if (_documentManager)
    connect(_documentManager->targetManager(), SIGNAL(targetChanged(TargetManager::TargetType,PerspectiveWidget*,QStringList)),
            this, SLOT(targetChanged(TargetManager::TargetType,PerspectiveWidget*,QStringList)));
}

ToolButton::~ToolButton() {
}

void ToolButton::setTool(QPointer<Tool> tool) {
  clearTool();
  _tool = tool;
  if (!_tool.isNull()) {
    connect(_tool.data(), SIGNAL(changed()), this, SLOT(toolChanged()));
    connect(_tool.data(), SIGNAL(triggered()), this, SLOT(toolTriggered()));
    if (!_tool->acceptedTargets().contains(_targetType))
      _targetType = _tool->preferredTarget();
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

void ToolButton::setGlobalKey(int key, Qt::KeyboardModifiers modifiers) {
  if (!_documentManager)
    return;
  if (key) {
    _documentManager.data()->setGlobalKey(key, this, modifiers);
    // LATER better keyLabel, including modifiers
    _keyLabel = (key > 32 && key < 128) ? QChar((char)key) : '?';
  } else {
    _keyLabel = QString();
  }
  toolChanged();
}

void ToolButton::toolChanged() {
  QString toolTip = _tool ? _tool.data()->label() : QString();
  if (!_keyLabel.isNull())
    toolTip.append(" (").append(_keyLabel).append(')');
  setToolTip(toolTip);
  _currentlyTriggerable = _tool ? _tool->triggerable(_targetType) : false;
  update();
}

void ToolButton::toolTriggered() {
  QPropertyAnimation *a = new QPropertyAnimation(this, "flashBackground");
  a->setDuration(300);
  a->setStartValue(QColor(Qt::white));
  a->setEndValue(QColor(Qt::lightGray));
  a->start(QAbstractAnimation::DeleteWhenStopped);
}

void ToolButton::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  //p.setRenderHint(QPainter::TextAntialiasing);
  p.setPen(Qt::black);
  p.drawRoundedRect(QRect(0, 0, 36, 36), 4, 4);
  p.setPen(Qt::darkGray);
  p.setBrush(_flashBackground != Qt::lightGray
             ? _flashBackground
             : _mousePressPoint.isNull()
               ? (_mouseCurrentlyOver ? Qt::darkGray : Qt::lightGray)
               : Qt::white);
  p.drawRoundedRect(QRect(1, 1, 34, 34), 4, 4);
  if (_tool)
    p.drawPixmap(2, 2, _tool->icon()
                 .pixmap(32, 32, _currentlyTriggerable
                         ? QIcon::Normal : QIcon::Disabled));
  if (!_keyLabel.isNull()) {
    QFont font("Sans");
    font.setPixelSize(12);
    p.setFont(font);
    p.setPen(Qt::black);
    p.drawText(QRectF(2, 34-12, 32, 12), Qt::AlignRight, _keyLabel);
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
    QFont font("Sans");
    font.setPixelSize(12);
    p.setFont(font);
    p.setPen(Qt::black);
    p.drawText(QRectF(2, 34-12, 32, 12), Qt::AlignLeft, targetIndicator);
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
      if (_tool && !_tool->acceptedTargets().contains(_targetType))
        _targetType = _tool->preferredTarget();
      break;
    default:
      ;
    }
    update();
  }
}

void ToolButton::trigger() {
  if (_tool)
    _tool->trigger(_targetType);
}

void ToolButton::enterEvent(QEvent *e) {
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
    md->setText(_tool.data()->label());
    md->setData(MIMETYPE_TOOL_ID, _tool.data()->id().toUtf8());
    d.setMimeData(md);
    QPixmap pm = _tool.data()->icon().pixmap(32);
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
  if (e->mimeData()->hasFormat(MIMETYPE_TOOL_ID)) {
    e->accept();
  } else
    e->ignore();
}

void ToolButton::dragMoveEvent(QDragMoveEvent *e) {
  // by default (i.e. without modifiers) move action if possible (i.e. if from
  // another toolbutton)
  // note that if shift is pressed, at less on Windows, the event is provided
  // with move action set as default by os or Qt framework
  if ((e->keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier
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
  QPointer<Tool> tool;
  if (!_documentManager.isNull()) {
    QString id = QString::fromUtf8(e->mimeData()->data(MIMETYPE_TOOL_ID));
    if (!id.isEmpty())
      tool = _documentManager.data()->toolById(id);
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
                               QStringList itemIds) {
  Q_UNUSED(perspectiveWidget)
  Q_UNUSED(itemIds)
  //qDebug() << "ToolButton::targetChanged" << (_tool ? _tool->id() : "-") << targetType << perspectiveWidget << itemIds;
  if (targetType == _targetType && _tool && _tool->enabled()) {
    bool triggerable = _tool->triggerable(targetType);
    //qDebug() << "--" << triggerable << _currentlyTriggerable;
    if (triggerable != _currentlyTriggerable)
      toolChanged();
  }
}
