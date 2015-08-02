#ifndef DTPACTION_H
#define DTPACTION_H

#include <QAction>
#include <QList>
#include <QString>
#include "libh6ncsu_global.h"
#include "targetmanager.h"

class QAction;
class PerspectiveWidget;
class DtpDocumentManager;

/** DtpAction is a DocumentManager-aware QAction, hence it is target-aware
  * and therefore able to change its behaviour or appearance depending on
  * TargetManager events.
  */
class LIBH6NCSUSHARED_EXPORT DtpAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(DtpAction)
  DtpDocumentManager *_documentManager;
  QString _id;

public:
  /** Become child of documentManager whose takes ownership. */
  DtpAction(DtpDocumentManager *documentManager, const QString id);
  DtpDocumentManager *documentManager() const { return _documentManager; }
  const QString id() const { return _id; }
};

#endif // DTPACTION_H
