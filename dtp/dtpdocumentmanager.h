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
#ifndef DTPDOCUMENTMANAGER_H
#define DTPDOCUMENTMANAGER_H

#include "targetmanager.h"
#include "modelview/shareduiitemdocumentmanager.h"

class QUndoStack;
class DtpAction;

#define MIMETYPE_ACTION_ID "pumpkin-lib/action-id"
#define MIMETYPE_ITEM_ID "pumpkin-lib/item-id"

/** DtpDocumentManager is a core class of the Document-Target-Perspective model,
 * this is a non-visible (i.e. non-widget) class for handling data and events
 * global to a given document.
 * This includes generic features such as: access to target manager, access to
 * undo stack...
 * Every application is expected to extends this class and add
 * application-specific features such as: load-save features, specific data
 * access...
 */
class LIBP6GUISHARED_EXPORT DtpDocumentManager
    : public SharedUiItemDocumentManager {
  Q_OBJECT
  Q_DISABLE_COPY(DtpDocumentManager)
  TargetManager *_targetManager;
  QUndoStack *_undoStack;
  bool _pushChangesToUndoStack = true;
  QMap<Utf8String,QPointer<DtpAction>> _actions;
  QList<QPointer<DtpAction>> _permanentActions;

public:
  explicit DtpDocumentManager(QObject *parent = 0);
  TargetManager *targetManager() { return _targetManager; }
  QUndoStack *undoStack() { return _undoStack; }
  using SharedUiItemDocumentManager::createNewItem;
  /** Override SharedUiItemDocumentManager::createNewItem() to push
   * QUndoCommand in QUndoStack when returning true. */
  SharedUiItem createNewItem(
      const Utf8String &qualifier, PostCreationModifier modifier,
      QString *errorString) override;
  /** Override SharedUiItemDocumentManager::changeItemByUiData() to push
   * QUndoCommand in QUndoStack when returning true and display error to the
   * user otherwise. */
  bool changeItemByUiData(
      const SharedUiItem &old_item, int section, const QVariant &value,
      QString *errorString = 0) override;
  /** Override SharedUiItemDocumentManager::changeItem() to push QUndoCommand in
   * QUndoStack when returning true. */
  bool changeItem(
      const SharedUiItem &new_item, const SharedUiItem &old_item,
      const Utf8String &qualifier, QString *errorString) override;
  bool pushChangesToUndoStack() const { return _pushChangesToUndoStack; }
  /** Push further changes to the undo stack.
   * Enabled by default.
   * Can be disabled e.g. when a change should not be made visible/undoable
   * to the user. */
  void setPushChangesToUndoStack(bool enable = true) {
    _pushChangesToUndoStack = enable; }
  void registerAction(DtpAction *action, bool is_permanent = false);
  DtpAction *actionById(Utf8String actionId) const;
  auto permanentActions() const { return _permanentActions; }
};

#endif // DTPDOCUMENTMANAGER_H
