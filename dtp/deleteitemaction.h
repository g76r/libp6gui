#ifndef DELETEITEMACTION_H
#define DELETEITEMACTION_H

#include "dtpaction.h"

class LIBH6NCSUSHARED_EXPORT DeleteItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(DeleteItemAction)

public:
  explicit DeleteItemAction(DtpDocumentManager *documentManager);
};

#endif // DELETEITEMACTION_H
