#ifndef DELETEITEMACTION_H
#define DELETEITEMACTION_H

#include "dtpaction.h"
#include "dtpdocumentmanager.h"

class LIBH6NCSUSHARED_EXPORT DeleteItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(DeleteItemAction)

public:
  DeleteItemAction(DtpDocumentManager *documentManager, QObject *parent);
  explicit DeleteItemAction(DtpDocumentManager *documentManager)
    : DeleteItemAction(documentManager, documentManager) { }
};

#endif // DELETEITEMACTION_H
