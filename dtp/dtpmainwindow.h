#ifndef DTPMAINWINDOW_H
#define DTPMAINWINDOW_H

#include <QMainWindow>
#include "libh6ncsu_global.h"

class DtpDocumentManager;

/** Document-Target-Perspective applications should have a special window which
 * closing triggers the application end. This window is the DtpMainWindow.
 * They must have only one DptMainWindow instance at a time, which is accessible
 * through DtpMainWindow::instance().
 *
 * When closing this window, document managers are asked to perform checks/save
 * operations if needed and all other top level windows are closed before this
 * one.
 *
 * Apart from dialogs, other top level widgets should be PerspectiveWidget that
 * have been popClone()'d.
 *
 * @see PerspectiveWidget
 */
class LIBH6NCSUSHARED_EXPORT DtpMainWindow : public QMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(DtpMainWindow)

private:
  QList<DtpDocumentManager*> _documentManagers; // LATER remove ?

public:
  explicit DtpMainWindow(QWidget *parent = 0);
  /** Syntaxic sugar. @see addDocumentManager(). */
  explicit DtpMainWindow(DtpDocumentManager *documentManager,
                         QWidget *parent = 0);
  ~DtpMainWindow();
  virtual void windowFocused();
  void closeEvent(QCloseEvent *event);
  QList<DtpDocumentManager*> documentManagers() const;
  void addDocumentManager(DtpDocumentManager *documentManager);
  static DtpMainWindow *instance();
  /** If item found: ensure visible and focus it. If item writeable: start
    * editing it.
    * Call recursively PerspectiveWidget::startItemEdition() among direct
    * PerspectiveWidget children if any. */
  bool startItemEdition(QString qualifiedId);
};

#endif // DTPMAINWINDOW_H
