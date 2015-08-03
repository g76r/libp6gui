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
    * @return a pointer on new widget */
  virtual PerspectiveWidget *popClone();
  /** If item found: ensure visible and focus it. If item writeable: start
    * editing it.
    * This method is called e.g. when an item has just been added interactively.
    */
  virtual void startItemEdition(QString itemId);
};

#endif // PERSPECTIVEWIDGET_H
