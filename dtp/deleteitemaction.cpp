#include "deleteitemaction.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(QIcon(":fa/trash.svg"));
  setText("Delete Item");
  connect(this, &DeleteItemAction::triggered, [=]() {
    if (!documentManager)
      return;
    foreach (const QString &qualifiedId,
             documentManager->targetManager()->targetItems()) {
      SharedUiItem oldItem = documentManager->itemById(qualifiedId);
      if (!oldItem.isNull())
        documentManager->changeItem(SharedUiItem(), oldItem,
                                    oldItem.idQualifier());
    }
  });
}

