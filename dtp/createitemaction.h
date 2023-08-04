/* Copyright 2015-2023 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * Libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * Libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CREATEITEMACTION_H
#define CREATEITEMACTION_H

#include "dtpaction.h"
#include "dtpdocumentmanager.h"
#include <functional>
#include "util/utf8string.h"

class LIBP6GUISHARED_EXPORT CreateItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(CreateItemAction)

private:
  SharedUiItemDocumentManager::PostCreationModifier _modifier;
  Utf8String _idQualifier;

public:
  CreateItemAction(
      DtpDocumentManager *documentManager, Utf8String idQualifier,
      QString text, QIcon icon, Utf8String actionId,
      TargetManager::TargetType targetType = TargetManager::PrimaryTarget,
      QObject *parent = 0);
  CreateItemAction(
      DtpDocumentManager *documentManager, Utf8String idQualifier,
      QString text, QIcon icon = QIcon(":fas/circle-plus.svg"),
      TargetManager::TargetType targetType = TargetManager::PrimaryTarget,
      QObject *parent = 0)
    : CreateItemAction(documentManager, idQualifier, text, icon,
                       "create_"+idQualifier, targetType, parent) { }
  CreateItemAction(
      DtpDocumentManager *documentManager, Utf8String idQualifier,
      QIcon icon = QIcon(":fas/circle-plus.svg"),
      TargetManager::TargetType targetType = TargetManager::PrimaryTarget,
      QObject *parent = 0)
    : CreateItemAction(
        documentManager, idQualifier, tr("Create %1").arg(idQualifier), icon,
        targetType, parent) {}
  void setPostCreationModifier(
      SharedUiItemDocumentManager::PostCreationModifier modifier) {
    _modifier = modifier; }

protected:
  void onTrigger(bool checked) override;
};

#endif // CREATEITEMACTION_H
