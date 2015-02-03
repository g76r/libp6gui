#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "libh6ncsu_global.h"

class DttDocumentManager;

// FIXME rename this class to DttMainWindow
/** Base class for non-perspective main windows in a Document-Target-Tool
 * application.
 * In a DTT application main windows must either inherit from DttMainWindow
 * or from PerspectiveWidget.
 * @see PerspectiveWidget
 */
class LIBH6NCSUSHARED_EXPORT MainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(MainWindow)

private:
  QPointer<DttDocumentManager> _documentManager;

public:
  explicit MainWindow(QWidget *parent = 0);
  /** Syntaxic sugar. @see setDocumentManager(). */
  explicit MainWindow(DttDocumentManager *documentManager, QWidget *parent = 0);
  ~MainWindow();
  void keyPressEvent(QKeyEvent *event);
  virtual void windowFocused();
  void closeEvent(QCloseEvent *event);
  DttDocumentManager *documentManager() const;
  /** DTT applications that manage only one document type should set a
   * DocumentManager on DttMainWindow to catch global keys at top level, but
   * applications managing several type of documents within the same main
   * window should not, to avoid user confusion between documents. */
  void setDocumentManager(DttDocumentManager *documentManager);
};

#endif // MAINWINDOW_H
