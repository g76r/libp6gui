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
#ifndef DELETEITEMACTION_H
#define DELETEITEMACTION_H

#include "dtpaction.h"
#include "dtpdocumentmanager.h"

class LIBP6GUISHARED_EXPORT DeleteItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(DeleteItemAction)

public:
  DeleteItemAction(
      DtpDocumentManager *documentManager, Utf8String actionId,
      TargetManager::TargetType targetType, QObject *parent);
  explicit DeleteItemAction(DtpDocumentManager *documentManager)
    : DeleteItemAction(documentManager, {}, TargetManager::PrimaryTarget,
                       documentManager) { }

protected:
  /** Subclasses can override this method to decide which items must not be
   * deleted.
   *
   * Default implementation: always return true (and thus delete any item).
   */
  virtual bool isThisItemDeletable(SharedUiItem item);

protected:
  void onTrigger(bool checked) override;
  void onTargetChanged(TargetManager::TargetType targetType,
                       PerspectiveWidget *perspectiveWidget,
                       Utf8StringList itemIds) override;
};

#endif // DELETEITEMACTION_H
