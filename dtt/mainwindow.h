#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "libh6ncsu_global.h"

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
  DocumentManager *documentManager() const;
  void setDocumentManager(DocumentManager *documentManager);
};

#endif // MAINWINDOW_H
