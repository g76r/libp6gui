#include "deleteitemaction.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(QIcon(":fa/trash.svg"));
  setText("Delete Item");
  connect(this, &DeleteItemAction::triggered, [=]() {
    foreach (const QString &qualifiedId,
             documentManager->targetManager()->targetItems()) {
      SharedUiItem oldItem = documentManager->itemById(qualifiedId);
      if (!oldItem.isNull())
        documentManager->changeItem(SharedUiItem(), oldItem,
                                    oldItem.idQualifier());
    }
  });
  connect(documentManager->targetManager(), &TargetManager::targetChanged,
          this, &DeleteItemAction::targetChanged);
  setEnabled(!documentManager->targetManager()->targetItems().isEmpty());
}

void DeleteItemAction::targetChanged(
    TargetManager::TargetType targetType, PerspectiveWidget *perspectiveWidget,
    QStringList itemIds) {
  Q_UNUSED(perspectiveWidget)
  if (targetType == TargetManager::PrimaryTarget)
    setEnabled(!itemIds.isEmpty());
}

