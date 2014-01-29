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
  GlobalKey gk = _globalKeys.value(event->key());
  if (gk.isNull() || gk._modifiers != event->modifiers())
    ; //qDebug() << "key not globally associated" << event->key()
      //         << event->text() << event->modifiers();
  else {
    Tool *t = _tools.value(gk._toolId).data();
    if (t) {
      qDebug() << "global key" << event->key() << "triggers tool" << gk._toolId;
      if (t->enabled())
        t->trigger();
      return true; // calling widget should not let its parent handle the event
    } else
      qWarning() << "global key associated to non existent tool" << event->key()
                 << event->text() << event->modifiers() << gk._toolId;
  }
  return false;
}

void DocumentManager::setGlobalKey(int key, QString toolId,
                                      Qt::KeyboardModifiers modifiers) {
  if (key && !toolId.isNull())
    _globalKeys.insert(key, GlobalKey(modifiers, toolId));
  else
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
