#ifndef CREATEITEMACTION_H
#define CREATEITEMACTION_H

#include "dtpaction.h"
#include "dtpdocumentmanager.h"

class LIBH6NCSUSHARED_EXPORT CreateItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(CreateItemAction)

public:
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier, QIcon icon,
      QString text, QObject *parent);
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier,
      QIcon icon = QIcon(":fa/plus_sign.svg"))
    : CreateItemAction(documentManager, idQualifier, icon,
                       "Create "+idQualifier, documentManager) { }
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier,
      QIcon icon, QObject *parent)
    : CreateItemAction(documentManager, idQualifier, icon,
                       "Create "+idQualifier, parent) { }
};

#endif // CREATEITEMACTION_H
