/* Copyright 2015-2017 Hallowyn and others.
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
#ifndef CREATEITEMACTION_H
#define CREATEITEMACTION_H

#include "dtpaction.h"
#include "dtpdocumentmanager.h"
#include <functional>

class LIBH6NCSUSHARED_EXPORT CreateItemAction : public DtpAction {
  Q_OBJECT
  Q_DISABLE_COPY(CreateItemAction)

public:
  using PostCreationModifier = std::function<void(SharedUiItem *newItem)>;

private:
  PostCreationModifier _modifier;

public:
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier, QIcon icon,
      QString text, QObject *parent);
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier,
      QIcon icon = QIcon(":fa/plus-circle.svg"))
    : CreateItemAction(documentManager, idQualifier, icon,
                       "Create "+idQualifier, documentManager) { }
  CreateItemAction(
      DtpDocumentManager *documentManager, QString idQualifier,
      QIcon icon, QObject *parent)
    : CreateItemAction(documentManager, idQualifier, icon,
                       "Create "+idQualifier, parent) { }
  void setPostCreationModifier(PostCreationModifier modifier) {
    _modifier = modifier; }
};

#endif // CREATEITEMACTION_H
