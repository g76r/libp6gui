/* Copyright 2014-2023 Hallowyn and others.
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
#include "dtpaction.h"
#include "dtpdocumentmanager.h"

DtpAction::DtpAction(
    DtpDocumentManager *documentManager, Utf8String actionId,
    TargetManager::TargetType targetType, QObject *parent)
  : QAction(parent), _documentManager(documentManager), _actionId(actionId),
    _targetType(targetType) {
  connect(this, &DtpAction::triggered, this, &DtpAction::onTrigger);
  if (!documentManager) // DM-free action like CloseAllPoppedWindowsAction
    return;
  connect(documentManager->targetManager(), &TargetManager::targetChanged,
          this, &DtpAction::onTargetChanged);
}

DtpDocumentManager *DtpAction::documentManager() const {
  return _documentManager;
}

void DtpAction::onTargetChanged(
    TargetManager::TargetType, PerspectiveWidget *, Utf8StringList) {
}

void DtpAction::onTrigger(bool) {
}

void DtpAction::setTargetType(TargetManager::TargetType targetType) {
  if (_targetType == targetType)
    return;
  emit targetTypeChanged(_targetType = targetType);
  if (!_documentManager)
    return;
  auto tm = _documentManager->targetManager();
  onTargetChanged(targetType, tm->targetWidget(targetType),
                  tm->targetItems(targetType));
}
