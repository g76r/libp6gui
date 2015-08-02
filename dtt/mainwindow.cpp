#include "mainwindow.h"
#include "dttdocumentmanager.h"
#include "tool.h"
#include <QLabel>
#include "dttdocumentmanager.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
}

MainWindow::MainWindow(DttDocumentManager *documentManager, QWidget *parent)
  : QMainWindow(parent) {
  setDocumentManager(documentManager);
}

MainWindow::~MainWindow() {
}

// LATER check if document should be saved and ignore event do prevent closing
void MainWindow::closeEvent(QCloseEvent *event) {
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

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (!_documentManager || !_documentManager->keyPressEvent(event))
    QMainWindow::keyPressEvent(event);
}

void MainWindow::windowFocused() {
}

DttDocumentManager *MainWindow::documentManager() const {
  return _documentManager.data();
}

void MainWindow::setDocumentManager(DttDocumentManager *documentManager) {
  _documentManager = documentManager;
  if (documentManager)
    documentManager->setMainWindow(this);
}
