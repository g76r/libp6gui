#include "createitemaction.h"
#include "dtpdocumentmanager.h"

CreateItemAction::CreateItemAction(
    DtpDocumentManager *documentManager, QString id, QString idQualifier,
    QIcon icon, QString text)
  : DtpAction(documentManager, id) {
  setIcon(icon);
  setText(text);
  connect(this, &CreateItemAction::triggered, [=]() {
    documentManager->createNewItem(idQualifier);
  });
}
