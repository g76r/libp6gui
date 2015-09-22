/* Copyright 2015 Hallowyn and others.
 * This file is part of libh6ncsu, see <https://gitlab.com/g76r/libh6ncsu>.
 * Libh6ncsu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libh6ncsu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libh6ncsu.  If not, see <http://www.gnu.org/licenses/>.
 */
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

