#ifndef CREATEITEMACTION_H
#define CREATEITEMACTION_H

#include "dtpaction.h"

class LIBH6NCSUSHARED_EXPORT CreateItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(CreateItemAction)

public:
  CreateItemAction(
      DtpDocumentManager *documentManager, QString id, QString idQualifier,
      QIcon icon, QString text);
  CreateItemAction(
      DtpDocumentManager *documentManager, QString id, QString idQualifier,
      QIcon icon = QIcon(":fa/plus_sign.svg"))
    : CreateItemAction(documentManager, id, idQualifier, icon,
                       "Create "+idQualifier) { }
};

#endif // CREATEITEMACTION_H
