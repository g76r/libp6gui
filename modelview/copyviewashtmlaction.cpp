/* Copyright 2016-2025 Hallowyn and others.
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
#include "copyviewashtmlaction.h"
#include <QApplication>
#include <QListView>
#include <QTreeView>
#include <QMimeData>
#include <QClipboard>
#include "format/stringutils.h"

// LATER add vertical (left) headers, especially for QTableView

CopyViewAsHtmlAction::CopyViewAsHtmlAction(
    QAbstractItemView *view, bool ignoreSelection, QObject *parent)
  : QAction(QIcon(":fas/copy.svg"), tr("Copy"), parent), _view(view),
    _indentationPlaceholder("&nbsp;&nbsp;&nbsp;"),
    _ignoreSelection(ignoreSelection) {
  connect(this, &QAction::triggered, [this]() {
    QClipboard *clipboard = QApplication::clipboard();
    auto *v = _view.data();
    if (!clipboard || !v)
      return;
    auto *m = v->model();
    if (!m)
      return;
    int columnCount = m->columnCount();
    QString html("<table>\n<tr>"), tsv;
    QSet<int> visibleColumns;
    auto *listView = qobject_cast<QListView*>(v);
    auto *treeView = qobject_cast<QTreeView*>(v);
    // LATER other views
    if (listView) {
      visibleColumns.insert(listView->modelColumn());
    } else if(treeView) {
      for (int i = 0; i < columnCount; ++i) {
        if (treeView->isColumnHidden(i))
          continue;
        visibleColumns.insert(i);
      }
    } else {
      for (int i = 0; i < columnCount; ++i)
        visibleColumns.insert(i);
    }
    for (int i = 0; i < columnCount; ++i) {
      if (!visibleColumns.contains(i))
        continue;
      html.append("<th>")
          .append(StringUtils::htmlEncode(m->headerData(i, Qt::Horizontal)
                                          .toString(), false))
          .append("</th>");
      tsv.append(m->headerData(i, Qt::Horizontal)
                 .toString().remove('\t'))
          .append('\t');
    }
    html.append("</tr>\n");
    tsv.chop(1);
    tsv.append('\n');
    recursiveCopy(html, tsv, visibleColumns, v, m, QModelIndex(), QString());
    html.append("</table>\n");
    QMimeData *md = new QMimeData;
    md->setHtml(html);
    md->setText(tsv);
    clipboard->setMimeData(md);
  });
}

void CopyViewAsHtmlAction::recursiveCopy(
    QString &html, QString &tsv, QSet<int> &visibleColumns,
    QAbstractItemView *v, QAbstractItemModel *m, const QModelIndex &parent,
    const QString &indentation) {
  auto *sm = v->selectionModel();
  int rowCount = m->rowCount(parent);
  //qDebug() << indentation << "recursivecopy" << sm << rowCount;
  for (int row = 0; row < rowCount; ++row) {
    // LATER also select parents of selected rows
    // LATER also hide invisible rows for QTableView
    if (!_ignoreSelection && sm && sm->hasSelection()
        && !sm->isRowSelected(row, parent))
      continue;
    html.append("<tr>");
    int columnCount = m->columnCount(parent);
    //qDebug() << indentation << "row" << row << columnCount;
    for (int column = 0; column < columnCount; ++column) {
      if (!visibleColumns.contains(column))
        continue;
      html.append("<td>");
      if (column == 0)
        html.append(indentation);
      html.append(StringUtils::htmlEncode(m->index(row, column, parent)
                                            .data().toString(), false))
          .append("</td>");
      tsv.append(m->index(row, column, parent)
                 .data().toString().remove('\t'))
          .append('\t');
    }
    html.append("</tr>\n");
    tsv.chop(1);
    tsv.append('\n');
    recursiveCopy(html, tsv, visibleColumns, v, m, m->index(row, 0, parent),
                  indentation+_indentationPlaceholder);
  }
}
