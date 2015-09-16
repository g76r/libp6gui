#include "deleteitemaction.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>
#include <QMessageBox>
#include "dtpmainwindow.h"

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(QIcon(":fa/trash-o.svg"));
  setText("Delete Item");
  connect(this, &DeleteItemAction::triggered, [=]() {
    foreach (const QString &qualifiedId,
             documentManager->targetManager()->targetItems()) {
      SharedUiItem oldItem = documentManager->itemById(qualifiedId);
      if (!oldItem.isNull()) {
        QString reason;
        QString idQualifier = oldItem.idQualifier();
        if (!documentManager->changeItem(
              SharedUiItem(), oldItem, idQualifier, &reason)) {
          // on error, warn user
          PerspectiveWidget *pw =
              documentManager->targetManager()->targetWidget();
          QMessageBox::warning(
                (pw ? (QWidget*)pw : (QWidget*)DtpMainWindow::instance()),
                tr("Cannot delete %1").arg(idQualifier),
                tr("Cannot delete %1.\n%2").arg(idQualifier).arg(reason));
          return;
        }
      }
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

