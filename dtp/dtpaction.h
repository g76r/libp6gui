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
#ifndef DTPACTION_H
#define DTPACTION_H

#include <QAction>
#include <QPointer>
#include "libp6gui_global.h"
#include "util/utf8string.h"
#include "targetmanager.h"

class DtpDocumentManager;

/** DtpAction is a DocumentManager-aware QAction, hence it is target-aware
  * and therefore able to change its behaviour or appearance depending on
  * TargetManager events.
  * Constructors connect triggered() signal to onTrigger() and
  * TargetManager::targetChanged() signal to onTargetChanged(), which do
  * nothing by default, so one can override them.
  */
class LIBP6GUISHARED_EXPORT DtpAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(DtpAction)
  Q_PROPERTY(TargetManager::TargetType targetType
             READ targetType
             WRITE setTargetType
             NOTIFY targetTypeChanged)
  QPointer<DtpDocumentManager> _documentManager;
  Utf8String _actionId;
  TargetManager::TargetType _targetType;

public:
  explicit DtpAction(
      DtpDocumentManager *documentManager, Utf8String actionId = {},
      TargetManager::TargetType targetType = TargetManager::PrimaryTarget,
      QObject *parent = 0);
  DtpAction(DtpDocumentManager *documentManager, QObject *parent)
    : DtpAction(documentManager, {}, TargetManager::PrimaryTarget, parent) { }
  DtpDocumentManager *documentManager() const;
  Utf8String actionId() const { return _actionId; }
  TargetManager::TargetType targetType() const { return _targetType; }
  void setTargetType(TargetManager::TargetType targetType);

signals:
  void targetTypeChanged(TargetManager::TargetType targetType);

protected:
  /** override this method ot perform the action */
  virtual void onTrigger(bool checked);
  /** overide this method if you want to react to a target change
   *  or a targetType change,
   *  common case is to setEnabled() depending on the new target */
  virtual void onTargetChanged(TargetManager::TargetType targetType,
                               PerspectiveWidget *perspectiveWidget,
                               Utf8StringList itemIds);
};

#endif // DTPACTION_H
