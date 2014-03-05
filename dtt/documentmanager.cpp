#include "documentmanager.h"
#include <QKeyEvent>
#include <QtDebug>
#include "perspectivewidget.h"
#include <QUndoStack>
#include "closeallpoppedwindowstool.h"
#include <QApplication>
#include "mainwindow.h"

// FIXME check remaining inconsistencies/duplicates between {Design,}DocumentManager

DocumentManager::DocumentManager(QObject *parent)
  : QObject(parent), _targetManager(new TargetManager(this)),
    _undoStack(new QUndoStack(this)) {
  addTool(new CloseAllPoppedWindowsTool(this), true);
  connect((QApplication*)(QCoreApplication::instance()),
          SIGNAL(focusChanged(QWidget*,QWidget*)),
          this, SLOT(focusChanged(QWidget*,QWidget*)));
}

DocumentManager::~DocumentManager() {
}

void DocumentManager::addTool(QPointer<Tool> tool, bool permanent){
  if (!tool.isNull()) {
    if (permanent)
      _permanentTools.append(tool);
    else
      setTempTool(tool); // FIXME check and fixes memory leaks since Qt5 incomplete code migration
    _tools.insert(tool.data()->id(), QPointer<Tool>(tool.data()));
  }
}

QPointer<Tool> DocumentManager::toolById(const QString id) {
  // TODO implement a kind of QWeakHash template that remove nulls on the fly
  QPointer<Tool> p = _tools.value(id);
  if (p.isNull())
    _tools.remove(id);
  return p;
}

void DocumentManager::registerWidget(PerspectiveWidget *widget) {
  _registredWidgets.insert(widget);
}

void DocumentManager::unregisterWidget(PerspectiveWidget *widget) {
  if (widget)
    _registredWidgets.remove(widget);
}

bool DocumentManager::keyPressEvent(QKeyEvent *event) {
  // LATER handle many keyboard layouts, which probably need to write
  // non-portable code, to catch QEvent::KeyboardLayoutChange, and to have
  // even worst hacks on MacOS than on other OSes
  // LATER handle several modifiers for the same key (allow to bind both ctl-A
  // and alt-A at the same time) or remove modifiers at all
  GlobalKey gk = _globalKeys.value(event->key());
  if (gk.isNull() || gk._modifiers != event->modifiers()) {
    qDebug() << "key not globally associated" << event->key()
             << event->text() << event->modifiers();
    return false;
  } else {
    if (gk._tool) {
      qDebug() << "global key" << event->key() << "triggers"
               << gk._tool.data();
      gk._tool->trigger();
    } else if (gk._toolButton) {
      qDebug() << "global key" << event->key() << "triggers"
               << gk._toolButton.data();
      gk._toolButton->trigger();
    } else {
      qWarning() << "global key associated to non existent trigger"
                 << event->key() << event->text() << event->modifiers();
      _globalKeys.remove(event->key());
    }
    event->accept();
    return true; // calling widget should not let its parent handle the event
  }
}

void DocumentManager::setGlobalKey(int key, QString toolId,
                                   Qt::KeyboardModifiers modifiers) {
  //qDebug() << "setGlobalKey" << key << toolId << modifiers;
  if (key && !toolId.isNull())
    _globalKeys.insert(key, GlobalKey(modifiers, _tools.value(toolId)));
  else
    qWarning() << "incorrect global key assignement:"<< key << toolId;
}

void DocumentManager::setGlobalKey(int key, ToolButton *toolButton,
                                   Qt::KeyboardModifiers modifiers) {
  qDebug() << "setGlobalKey" << key << toolButton << modifiers;
  if (key && toolButton)
    _globalKeys.insert(key, GlobalKey(modifiers, toolButton));
  else
    qWarning() << "incorrect global key assignement:"<< key << toolButton;
}

void DocumentManager::clearGlobalKey(int key) {
  _globalKeys.remove(key);
}

void DocumentManager::focusChanged(QWidget *oldWidget, QWidget *newWidget) {
  if (oldWidget && newWidget && oldWidget->window() != newWidget->window()) {
    /*qDebug() << "focus changed from TLW, old:"
             << oldWidget->window()->windowTitle()
             << "new:" << newWidget->window()->windowTitle();*/
    targetManager()->setTarget();
    PerspectiveWidget *pw = qobject_cast<PerspectiveWidget*>(
          newWidget->window());
    if (pw) {
      pw->windowFocused();
      return;
    }
    MainWindow *mw = qobject_cast<MainWindow*>(newWidget->window());
    if (mw) {
      mw->windowFocused();
      return;
    }
    qWarning() << "focus changed to unsupported top level window:"
               << newWidget->objectName();
  }
}

MainWindow *DocumentManager::mainWindow() const {
  return _mainWindow;
}

void DocumentManager::setMainWindow(MainWindow *mainWindow) {
  _mainWindow = mainWindow;
}
