/* Copyright 2016-2023 Hallowyn and others.
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
#ifndef COPYVIEWASHTMLACTION_H
#define COPYVIEWASHTMLACTION_H

#include "libp6gui_global.h"
#include <QAction>
#include <QAbstractItemView>
#include <QPointer>

/** Copy the content of a QAbstractItemView as HTML to the clipboard.
 *
 * Copy only selected item if any, or the whole content otherwise.
 */
class LIBP6GUISHARED_EXPORT CopyViewAsHtmlAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(CopyViewAsHtmlAction)
  QPointer<QAbstractItemView> _view;
  QString _indentationPlaceholder;
  bool _ignoreSelection;

public:
  CopyViewAsHtmlAction(QObject *parent = 0)
    : CopyViewAsHtmlAction(0, false, parent) { }
  CopyViewAsHtmlAction(QAbstractItemView *view, QObject *parent = 0)
    : CopyViewAsHtmlAction(view, false, parent) { }
  CopyViewAsHtmlAction(bool ignoreSelection, QObject *parent = 0)
    : CopyViewAsHtmlAction(0, ignoreSelection, parent) { }
  CopyViewAsHtmlAction(QAbstractItemView *view, bool ignoreSelection,
                       QObject *parent = 0);
  QString indentationPlaceholder() const { return _indentationPlaceholder; }
  void setIndentationPlaceholder(QString indentationPlaceholder) {
    _indentationPlaceholder = indentationPlaceholder; }
  void setView(QAbstractItemView *view) { _view = view; }

private:
  void recursiveCopy(
      QString &html, QSet<int> &visibleColumns, QAbstractItemView *v,
      QAbstractItemModel *m, QModelIndex parent, QString indentation);
};


#endif // COPYVIEWASHTMLACTION_H
