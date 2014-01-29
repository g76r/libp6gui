#ifndef PERSPECTIVEWIDGET_H
#define PERSPECTIVEWIDGET_H

#include <QWidget>
#include <QPointer>
#include "libh6ncsu_global.h"

class DocumentManager;
class DocumentVersion;
class Perspective;
class ItemPosition;

/** Parent class for widgets/forms offer a GUI on the document through a
  * perspective.
  * @see DocumentManager */
class LIBH6NCSUSHARED_EXPORT PerspectiveWidget : public QWidget {
  Q_OBJECT
private:
  QPointer<DocumentManager> _documentManager;

public:
  explicit PerspectiveWidget(QWidget *parent);
  virtual ~PerspectiveWidget();
  virtual void setDocumentManager(DocumentManager *documentManager);
  QPointer<DocumentManager> documentManager() const;
  /** Intercept keystrokes to give them to DocumentManager to handle global
    * key shortcuts.
    */
  void keyPressEvent(QKeyEvent *event);
  /** Called when a widget from the same top level widget receives focus.
    * The widget should update primary target to make it track itself as a
    * widget and its current selection as items.
    */
  virtual void windowFocused() = 0;
  /** Id of currently displayed perspective */
  virtual QString currentPerspectiveId() const = 0;

public slots:
  /** Create and a new identical widget (clone) showing same perspective and
    * make it visible as a top level widget (pop). */
  virtual void popClone() = 0;
  /** If item found: ensure visible and focus it. If item writeable: start
    * editing it.
    * This method is called e.g. when an item has just been added interactively.
    */
  virtual void startItemEdition(QString itemId);
};

#endif // PERSPECTIVEWIDGET_H
