#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "libh6ncsu_global.h"
#include "documentmanager.h"

class DocumentManager;

// FIXME rename this class
class LIBH6NCSUSHARED_EXPORT MainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

private:
  QPointer<DocumentManager> _documentManager;

public:
  explicit MainWindow(QWidget *parent = 0);
  explicit MainWindow(DocumentManager *documentManager, QWidget *parent = 0);
  ~MainWindow();
  void keyPressEvent(QKeyEvent *event);
  virtual void windowFocused();
  void closeEvent(QCloseEvent *event);
  QPointer<DocumentManager> documentManager() const { return _documentManager; }
  void setDocumentManager(QPointer<DocumentManager> documentManager) {
    _documentManager = documentManager; }
  void setDocumentManager(DocumentManager *documentManager) {
    _documentManager = documentManager; }
};

#endif // MAINWINDOW_H
