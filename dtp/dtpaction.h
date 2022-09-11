/* Copyright 2014-2022 Hallowyn and others.
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
#include <QList>
#include <QString>
#include "libp6gui_global.h"
#include "targetmanager.h"

class QAction;
class PerspectiveWidget;
class DtpDocumentManager;

/** DtpAction is a DocumentManager-aware QAction, hence it is target-aware
  * and therefore able to change its behaviour or appearance depending on
  * TargetManager events.
  */
class LIBP6GUISHARED_EXPORT DtpAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(DtpAction)
  DtpDocumentManager *_documentManager;

public:
  explicit DtpAction(DtpDocumentManager *documentManager, QObject *parent = 0);
  DtpDocumentManager *documentManager() const { return _documentManager; }
};

#endif // DTPACTION_H
