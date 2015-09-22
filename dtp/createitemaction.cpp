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
#include "createitemaction.h"
#include "dtpdocumentmanager.h"
#include "dtpmainwindow.h"
#include "perspectivewidget.h"
#include <QMessageBox>

CreateItemAction::CreateItemAction(
    DtpDocumentManager *documentManager, QString idQualifier, QIcon icon,
    QString text, QObject *parent)
  : DtpAction(documentManager, parent) {
  setIcon(icon);
  setText(text);
  connect(this, &CreateItemAction::triggered, [=]() {
    QString reason;
    SharedUiItem newItem =
        documentManager->createNewItem(idQualifier, &reason);
    PerspectiveWidget *pw = documentManager->targetManager()->targetWidget();
    // on error, warn user
    if (newItem.isNull()) {
      QMessageBox::warning(
            (pw ? (QWidget*)pw : (QWidget*)DtpMainWindow::instance()),
            tr("Cannot create %1").arg(idQualifier),
            tr("Cannot create %1.\n%2").arg(idQualifier).arg(reason));
      return;
    }
    // if a target PerspectiveWidget exists try to start item edition through it
    if (pw && pw->startItemEdition(newItem.qualifiedId()))
      return;
    // otherwise, or if it failed, try through main window
    DtpMainWindow *mainWindow = DtpMainWindow::instance();
    if (mainWindow)
      mainWindow->startItemEdition(newItem.qualifiedId());
  });
}
