#include "mainwindow.h"
#include "documentmanager.h"
#include "tool.h"
#include <QLabel>
#include "documentmanager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
}

MainWindow::MainWindow(DocumentManager *documentManager, QWidget *parent)
  : QMainWindow(parent), _documentManager(documentManager) {
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // LATER check if document should be saved here and ignore event do prevent closing
  // Note: creating a label here is a hack to ensure that MainWindow and chidren
  // destructors will all be called, even on platforms (at less Windows) where
  // closing the last window kills the program
  QLabel *label = new QLabel(tr("Shuting down..."));
  label->setAttribute(Qt::WA_DeleteOnClose);
  label->setAttribute(Qt::WA_QuitOnClose);
  label->show();
  QMetaObject::invokeMethod(label, "deleteLater", Qt::QueuedConnection);
  QMainWindow::closeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  _documentManager->keyPressEvent(event);
}

void MainWindow::windowFocused() {
}
