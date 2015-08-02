#include "dtpmainwindow.h"
#include "dtpdocumentmanager.h"
#include "dtpaction.h"
#include <QLabel>
#include <QApplication>

static DtpMainWindow *singletonInstance = 0;

DtpMainWindow::DtpMainWindow(QWidget *parent) : QMainWindow(parent) {
  singletonInstance = this;
}

DtpMainWindow::DtpMainWindow(DtpDocumentManager *documentManager,
                             QWidget *parent)
  : QMainWindow(parent) {
  addDocumentManager(documentManager);
}

DtpMainWindow::~DtpMainWindow() {
  singletonInstance = 0;
}

// LATER check if document should be saved and ignore event do prevent closing
void DtpMainWindow::closeEvent(QCloseEvent *event) {
  // closing subwindows before main window is needed because subwindows may
  // rely on resources allocated by main window, such as the DocumentManager,
  // some QAbstractItemModels, loaded configuration, etc.
  foreach (QWidget *w, QApplication::topLevelWidgets()) {
    auto pw = qobject_cast<PerspectiveWidget*>(w);
    if (pw)
      pw->close();
  }
  // Note: creating a label here is a hack to ensure that MainWindow and chidren
  // destructors will all be called, even on platforms (at less Windows) where
  // closing the last window kills the program
  QLabel *label = new QLabel(tr("Shuting down..."));
  label->setAttribute(Qt::WA_DeleteOnClose);
  label->setAttribute(Qt::WA_QuitOnClose);
  label->setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
  label->setMargin(20);
  label->move(x()+width()/2, y()+height()/2);
  label->show();
  QMetaObject::invokeMethod(label, "deleteLater", Qt::QueuedConnection);
  QMainWindow::closeEvent(event);
}

void DtpMainWindow::windowFocused() {
}

QList<DtpDocumentManager*> DtpMainWindow::documentManagers() const {
  return _documentManagers;
}

void DtpMainWindow::addDocumentManager(DtpDocumentManager *documentManager) {
  _documentManagers.append(documentManager);
}

DtpMainWindow *DtpMainWindow::instance() {
  return singletonInstance;
}
