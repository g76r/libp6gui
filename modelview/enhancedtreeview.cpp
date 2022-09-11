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
#include "enhancedtreeview.h"
#include <QKeyEvent>

EnhancedTreeView::EnhancedTreeView(QWidget *parent) : QTreeView(parent) {
}

void EnhancedTreeView::leaveEvent(QEvent *event) {
  QTreeView::leaveEvent(event);
  emit leaved();
}

void EnhancedTreeView::keyPressEvent(QKeyEvent *event) {
  if (_ignoreKeyboardInput)
    event->setAccepted(false);
  else
    QTreeView::keyPressEvent(event);
}

void EnhancedTreeView::setModel(QAbstractItemModel *model) {
  QAbstractItemModel *old = this->model();
  if (old) {
    disconnect(model, &QAbstractItemModel::modelReset,
               this, &EnhancedTreeView::rowsAppeared);
    disconnect(model, &QAbstractItemModel::rowsInserted,
               this, &EnhancedTreeView::rowsAppeared);
    disconnect(model, &QAbstractItemModel::rowsMoved,
               this, &EnhancedTreeView::rowsAppeared);
  }
  QTreeView::setModel(model);
  if (model) {
    connect(model, &QAbstractItemModel::modelReset,
            this, &EnhancedTreeView::rowsAppeared);
    connect(model, &QAbstractItemModel::rowsInserted,
            this, &EnhancedTreeView::rowsAppeared);
    connect(model, &QAbstractItemModel::rowsMoved,
            this, &EnhancedTreeView::rowsAppeared);
    rowsAppeared();
  }
}

void EnhancedTreeView::dataChanged(
    const QModelIndex &topLeft, const QModelIndex &bottomRight,
    const QVector<int> &roles) {
  QTreeView::dataChanged(topLeft, bottomRight, roles);
  rowsAppearedOrChanged();
}

void EnhancedTreeView::rowsAppeared() {
  if (_expandToDepthOnChange >= 0)
    expandToDepth(_expandToDepthOnChange);
  else
    expandAll();
  rowsAppearedOrChanged();
}

void EnhancedTreeView::rowsAppearedOrChanged() {
  if (_resizeColumnsToContentsOnChange) {
    auto *m = model();
    int count = m ? m->columnCount() : 0;
    for (int i = 0; i < count; ++i)
      if (!isColumnHidden(i))
        resizeColumnToContents(i);
  }
}

inline static QItemSelectionModel::SelectionFlags selectionBehaviorFlags(
    QAbstractItemView::SelectionBehavior selectionBehavior) {
  switch (selectionBehavior) {
  case QAbstractItemView::SelectRows:
    return QItemSelectionModel::Rows;
  case QAbstractItemView::SelectColumns:
    return QItemSelectionModel::Columns;
  case QAbstractItemView::SelectItems:
    ;
  }
  return QItemSelectionModel::NoUpdate;
}

void EnhancedTreeView::openNextEditor(CursorAction direction) {
  QModelIndex current = currentIndex(), next;
  if (_editNextMeansEditRight) {
    QAbstractItemModel *m = model();
    if (m) {
      int row = current.row(), col = current.column();
      do {
        if (direction == MovePrevious) {
          if (col == 0) {
            --row;
            col = m->columnCount(current.parent())-1;
          } else
            --col;
        } else {
          if (col == m->columnCount(current.parent())-1) {
            ++row;
            col = 0;
          } else
            ++col;
        }
        next = m->index(row, col, current.parent());
      } while (next.isValid() && isColumnHidden(col));
    }
  } else {
    next = moveCursor(direction, Qt::NoModifier);
  }
  if (next.isValid()) {
    QPersistentModelIndex persistent(next);
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::NoUpdate;
    QItemSelectionModel *sm = selectionModel();
    if (sm) {
      if (selectionMode() != NoSelection)
        flags = QItemSelectionModel::ClearAndSelect
            | selectionBehaviorFlags(selectionBehavior());
      sm->setCurrentIndex(persistent, flags);
    }
    if (next.flags() & Qt::ItemIsEditable
        && (!(editTriggers() & QAbstractItemView::CurrentChanged)))
      edit(persistent);
  } else {
    setCurrentIndex(current);
  }
}

void EnhancedTreeView::closeEditor(
    QWidget *editor, QAbstractItemDelegate::EndEditHint hint) {
  switch (hint) {
  case QAbstractItemDelegate::EditNextItem:
    QTreeView::closeEditor(editor, QAbstractItemDelegate::NoHint);
    openNextEditor(MoveNext);
    return;
  case QAbstractItemDelegate::EditPreviousItem:
    QTreeView::closeEditor(editor, QAbstractItemDelegate::NoHint);
    openNextEditor(MovePrevious);
    return;
  default:
    ;
  }
  QTreeView::closeEditor(editor, hint);
}
