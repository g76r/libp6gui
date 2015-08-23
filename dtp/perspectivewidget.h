#ifndef PERSPECTIVEWIDGET_H
#define PERSPECTIVEWIDGET_H

#include <QWidget>
#include "libh6ncsu_global.h"

class DtpDocumentManager;
class DocumentVersion;
class Perspective;
class ItemPosition;
class TargetManager;

/** Parent class for widgets/forms offering a GUI on the document through a
  * perspective.
  * Apart from holding the association to a DocumentManager and being resposible
  * for providing the currently displayed perspective id, the class provides
  * interaction behavior such as being clonable as a top level widget (= a
  * standalone window).
  * @see DocumentManager */
class LIBH6NCSUSHARED_EXPORT PerspectiveWidget : public QWidget {
  Q_OBJECT
private:
  DtpDocumentManager *_documentManager;

public:
  explicit PerspectiveWidget(QWidget *parent);
  virtual void setDocumentManager(DtpDocumentManager *documentManager);
  DtpDocumentManager *documentManager() const;
  /** Id of currently displayed perspective */
  virtual QString currentPerspectiveId() const = 0;
  /** Convenience method */
  TargetManager *targetManager() {return targetManager(this); }
  /** Convenience method */
  static TargetManager *targetManager(PerspectiveWidget *pw);

public slots:
  /** Create and a new identical widget (clone) showing same perspective and
    * make it visible as a top level widget (pop).
    * If child class has a Q_INVOKABLE default constructor, use it to create
    * a new instance, set the new instance to the same DocumentManager,
    * set Qt::WA_DeleteOnClose attribute, unset Qt::WA_QuitOnClose attribute
    * and show the widget (as a top level window).
    * @return a pointer on new widget
    * @see copyCloneSharedData() */
  // LATER remove virtual and final keywords
  virtual PerspectiveWidget *popClone() final;
  /** If item found: ensure visible and set it current. If item writeable: start
    * editing it.
    * In this widget or, if not found, recursively within its children, or, if
    * still not found, recursively within its ancestors and siblings.
    * This method is called e.g. when an item has just been added interactively.
    */
  // LATER remove virtual and final keywords
  virtual bool startItemEdition(QString qualifiedId) final;

protected:
  /** If item found: ensure visible and set it current. If item writeable: start
    * editing it.
    * This method is called through startItemEdition() e.g. when an item has
    * just been added interactively.
    * Implementation must not recursively try to call children or parent widgets
    * since this is already done by startItemEdition(), it must only start
    * edition on one of its item views, e.g. calling startItemEdition on the
    * right DtpTreeView depending on item's id qualifier.
    */
  virtual bool startItemEditionHere(QString qualifiedId);
  /** Copy data shared among cloned perspectives. Called by popClone().
   * Default impl: copy document manager and window icon.
   * Can be extended to copy additionnal data/resources before or after.
   * @param newWidget newly created widget, not yet shown.
   * @see popClone() */
  virtual void copyCloneSharedData(PerspectiveWidget *newWidget) const;

private:
  /** Used internally by startItemEdition() */
  inline bool recursiveStartItemEdition(
      QString qualifiedId, PerspectiveWidget *callingChild = 0);
  /** Used by recursiveStartItemEdition to find PerspectiveWidget within
   * non-PerspectiveWidget descendants. */
  static inline bool recursiveStartItemEditionThroughNonPW(
      QString qualifiedId, QWidget *widget);
};

#endif // PERSPECTIVEWIDGET_H
