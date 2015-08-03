#include "createitemaction.h"
#include "dtpdocumentmanager.h"

CreateItemAction::CreateItemAction(
    DtpDocumentManager *documentManager, QString idQualifier, QIcon icon,
    QString text, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(icon);
  setText(text);
  connect(this, &CreateItemAction::triggered, [=]() {
    documentManager->createNewItem(idQualifier);
  });
}
