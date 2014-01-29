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
    _mouseover(false), _key(0), _modifiers(Qt::NoModifier),
    _flashBackground(Qt::lightGray) {
  setMaximumSize(36, 36);
  setMinimumSize(36, 36);
  setAcceptDrops(true);
}

ToolButton::~ToolButton() {
}

void ToolButton::setTool(QPointer<Tool> tool) {
  clearTool();
  _tool = tool;
  if (!_tool.isNull()) {
    connect(_tool.data(), SIGNAL(changed()), this, SLOT(toolChanged()));
    connect(_tool.data(), SIGNAL(triggered()), this, SLOT(toolTriggered()));
    toolChanged();
  }
  if (_key && _documentManager)
    _documentManager.data()
        ->setGlobalKey(_key, _tool ? _tool.data()->id() : QString(),
                       _modifiers);
}

void ToolButton::clearTool() {
  if (!_tool.isNull()) {
    QObject::disconnect(_tool.data(), SIGNAL(changed()),
                        this, SLOT(toolChanged()));
    QObject::disconnect(_tool.data(), SIGNAL(triggered()),
                        this, SLOT(toolTriggered()));
    if (_key && _documentManager)
      _documentManager.data()
          ->setGlobalKey(_key, QString(), _modifiers);
  }
  _tool.clear();
}

void ToolButton::setGlobalKey(int key, Qt::KeyboardModifiers modifiers) {
  if (!_documentManager)
    return;
  if (key) {
    _documentManager.data()
        ->setGlobalKey(_key = key, _tool ? _tool.data()->id() : QString(),
                       _modifiers = modifiers);
    // LATER better keyLabel, including modifiers
    _keyLabel = (_key > 32 && _key < 128) ? QChar((char)_key) : '?';
  } else {
    _documentManager.data()->setGlobalKey(_key, QString(), _modifiers);
    _keyLabel = QString();
  }
  toolChanged();
}

void ToolButton::toolChanged() {
  QString toolTip = _tool ? _tool.data()->label() : QString();
  if (!_keyLabel.isNull())
    toolTip.append(" (").append(_keyLabel).append(')');
  setToolTip(toolTip);
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
               ? (_mouseover ? Qt::darkGray : Qt::lightGray)
               : Qt::white);
  p.drawRoundedRect(QRect(1, 1, 34, 34), 4, 4);
  if (_tool)
    p.drawPixmap(2, 2, _tool.data()
                 ->icon().pixmap(32, 32, _tool.data()->enabled()
                                 ? QIcon::Normal : QIcon::Disabled));
  if (!_keyLabel.isNull()) {
    QFont font("Sans");
    font.setPixelSize(12);
    p.setFont(font);
    p.setPen(Qt::blue);
    p.drawText(QRectF(2, 34-12, 32, 12), Qt::AlignRight, _keyLabel);
  }
  p.end();
}

void ToolButton::mousePressEvent(QMouseEvent *e) {
  QAbstractButton::mousePressEvent(e);
  _mousePressPoint = e->pos();
  update();
}

void ToolButton::mouseReleaseEvent(QMouseEvent *e) {
  QAbstractButton::mouseReleaseEvent(e);
  //qDebug() << "release";
  _mousePressPoint = QPoint();
  if (rect().contains(e->pos()) && _tool)
    _tool.data()->trigger();
  update();
}

void ToolButton::enterEvent(QEvent *e) {
  QAbstractButton::enterEvent(e);
  _mouseover = true;
  update();
}

void ToolButton::leaveEvent(QEvent *e) {
  QAbstractButton::leaveEvent(e);
  _mouseover = false;
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
