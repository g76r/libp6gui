/* Copyright 2016 Hallowyn and others.
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
#ifndef COPYVIEWASHTMLACTION_H
#define COPYVIEWASHTMLACTION_H

#include "libh6ncsu_global.h"
#include <QAction>
#include <QAbstractItemView>
#include <QPointer>

class QAbstractItemModel;

/** Copy the content of a QAbstractItemView as HTML to the clipboard.
 *
 * Copy only selected item if any, or the whole content otherwise.
 */
class LIBH6NCSUSHARED_EXPORT CopyViewAsHtmlAction : public QAction {
  Q_OBJECT
  Q_DISABLE_COPY(CopyViewAsHtmlAction)
  QPointer<QAbstractItemView> _view;
  QString _indentationPlaceholder;

public:
  CopyViewAsHtmlAction(QObject *parent = 0)
    : CopyViewAsHtmlAction(0, parent) { }
  CopyViewAsHtmlAction(QAbstractItemView *view, QObject *parent = 0);
  QString indentationPlaceholder() const { return _indentationPlaceholder; }
  void setIndentationPlaceholder(QString indentationPlaceholder) {
    _indentationPlaceholder = indentationPlaceholder; }
  void recursiveCopy(
      QString &html, QSet<int> &visibleColumns, QAbstractItemView *v,
      QAbstractItemModel *m, QModelIndex parent, QString indentation);
};


#endif // COPYVIEWASHTMLACTION_H
