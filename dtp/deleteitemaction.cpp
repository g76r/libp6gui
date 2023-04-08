/* Copyright 2015-2023 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "deleteitemaction.h"
#include "dtpdocumentmanager.h"
#include <QtDebug>
#include <QMessageBox>
#include "dtpmainwindow.h"
#include "util/undotransaction.h"

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(QIcon(":fa/trash-o.svg"));
  setText("Delete Item");
  connect(this, &DeleteItemAction::triggered, [this,documentManager]() {
    int count = documentManager->targetManager()->targetItems().size();
    if (count < 1)
      return;
    UndoTransaction transaction(
          documentManager, tr("Deleting %1 items.").arg(count));
    for (auto qualifiedId:
         documentManager->targetManager()->targetItems()) {
      SharedUiItem oldItem = documentManager->itemById(qualifiedId);
      if (!oldItem.isNull() && isThisItemDeletable(oldItem)) {
        QString reason;
        auto idQualifier = oldItem.idQualifier();
        if (!documentManager->changeItem(
              SharedUiItem(), oldItem, idQualifier, &reason)) {
          // on error, warn user
          PerspectiveWidget *pw =
              documentManager->targetManager()->targetWidget();
          QMessageBox::warning(
                (pw ? (QWidget*)pw : (QWidget*)DtpMainWindow::instance()),
                tr("Cannot delete %1").arg(idQualifier),
                tr("Cannot delete %1.\n%2").arg(idQualifier).arg(reason));
          return; // implicit rollback
        }
      }
    }
    transaction.commit();
  });
  connect(documentManager->targetManager(), &TargetManager::targetChanged,
          this, &DeleteItemAction::targetChanged);
  setEnabled(!documentManager->targetManager()->targetItems().isEmpty());
}

void DeleteItemAction::targetChanged(
    TargetManager::TargetType targetType, PerspectiveWidget *perspectiveWidget,
    QByteArrayList itemIds) {
  Q_UNUSED(perspectiveWidget)
  if (targetType == TargetManager::PrimaryTarget)
    setEnabled(!itemIds.isEmpty());
}

bool DeleteItemAction::isThisItemDeletable(SharedUiItem item) {
  Q_UNUSED(item)
  return true;
}

