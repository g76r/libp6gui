#include "deleteitemaction.h"
#include "dtpdocumentmanager.h"
#include "modelview/genericshareduiitem.h"
#include <QtDebug>

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(QIcon(":fa/trash.svg"));
  setText("Delete Item");
  connect(this, &DeleteItemAction::triggered, [=]() {
    if (!documentManager)
      return;
    //qDebug() << "DeleteItemAction" << documentManager->targetManager()->targetItems();
    foreach (const QString &qualifiedId,
             documentManager->targetManager()->targetItems())
      documentManager->changeItem(SharedUiItem(),
                                  GenericSharedUiItem(qualifiedId));
  });
}

