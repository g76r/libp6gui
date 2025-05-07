/* Copyright 2015-2025 Hallowyn and others.
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
#include "dtpmainwindow.h"
#include "util/undotransaction.h"
#include <QMessageBox>

DeleteItemAction::DeleteItemAction(
    DtpDocumentManager *documentManager, Utf8String actionId,
    TargetManager::TargetType targetType, QObject *parent)
  : DtpAction(documentManager, actionId, targetType, parent) {
  setIcon(QIcon(":far/trash-can.svg"));
  setText("Delete Item");
  setEnabled(documentManager->targetManager()->targetItems(targetType).size());
}

void DeleteItemAction::onTrigger(bool) {
  auto dm = documentManager();
  if (!dm)
    return;
  auto items = dm->targetManager()->targetItems(targetType());
  int count = items.size();
  if (count < 1)
    return;
  UndoTransaction transaction(
        dm, tr("Deleting %1 items: %2%3").arg(count).arg(items[0])
      .arg(count > 1 ? "..." : ""));
  for (const auto &qualifiedId: items) {
    SharedUiItem oldItem = dm->itemById(qualifiedId);
    if (!oldItem.isNull() && isThisItemDeletable(oldItem)) {
      QString reason;
      auto qualifier = oldItem.qualifier();
      if (!dm->changeItem(
            SharedUiItem(), oldItem, qualifier, &reason)) {
        // on error, warn user
        PerspectiveWidget *pw =
            dm->targetManager()->targetWidget();
        QMessageBox::warning(
              (pw ? (QWidget*)pw : (QWidget*)DtpMainWindow::instance()),
              tr("Cannot delete %1").arg(qualifier),
              tr("Cannot delete %1.\n%2").arg(qualifier).arg(reason));
        return; // implicit rollback
      }
    }
  }
  transaction.commit();
}

void DeleteItemAction::onTargetChanged(
    TargetManager::TargetType targetType, PerspectiveWidget *,
    Utf8StringList itemIds) {
  if (targetType != this->targetType())
    return;
  setEnabled(itemIds.size());
}

bool DeleteItemAction::isThisItemDeletable(SharedUiItem item) {
  Q_UNUSED(item)
  return true;
}

