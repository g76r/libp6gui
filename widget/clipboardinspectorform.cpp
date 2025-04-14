/* Copyright 2025 Grégoire Barbier and others.
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
#include "clipboardinspectorform.h"
#include "ui_clipboardinspectorform.h"
#include <QClipboard>
#include <QMimeData>

ClipboardInspectorForm::ClipboardInspectorForm(QWidget *parent) :
  QWidget(parent), ui(new Ui::ClipboardInspectorForm),
  _clipboard(QGuiApplication::clipboard()) {
  ui->setupUi(this);
  setWindowIcon(QIcon(":far/clipboard.svg"));
  ui->table->setColumnCount(2);
  ui->table->setHorizontalHeaderLabels({"mime type", "data"});
  connect(ui->clear_button, &QPushButton::clicked,
          this, &ClipboardInspectorForm::clear_clipboard);
  connect(_clipboard, &QClipboard::dataChanged,
          this, &ClipboardInspectorForm::update_display);
  update_display();
  ui->table->setContextMenuPolicy(Qt::ActionsContextMenu);
  auto remove_action = new QAction("Remove selected formats", this);
  remove_action->setIcon(QIcon(":far/trash-can.svg"));
  connect(remove_action, &QAction::triggered, [this](){
    auto old_md = _clipboard->mimeData();
    if (!old_md)
      return;
    auto new_md = new QMimeData;
    QStringList removed_formats;
    for (auto item: ui->table->selectedItems()) // TODO do better
      removed_formats.append(item->text());
    for (auto format: old_md->formats())
      if (!removed_formats.contains(format))
        new_md->setData(format, old_md->data(format));
    _clipboard->setMimeData(new_md);
  });
  ui->table->addAction(remove_action);
}

ClipboardInspectorForm::~ClipboardInspectorForm() {
  delete ui;
}

void ClipboardInspectorForm::update_display() {
  auto md = _clipboard->mimeData();
  ui->table->setRowCount(0);
  if (md) {
    for (auto format: md->formats()) {
      int row = ui->table->rowCount();
      ui->table->insertRow(row);
      ui->table->setItem(row, 0, new QTableWidgetItem(format));
      auto data = md->data(format);
      if (format.startsWith("text/")
          || format == "application/x-qt-windows-mime;value=\"Rich Text Format\""
          || format == "application/x-qt-windows-mime;value=\"Richtext Format\""
          || format == "application/x-qt-windows-mime;value=\"HTML (HyperText Markup Language)\""
          )
        ui->table->setItem(row, 1, new QTableWidgetItem(data));
      else
        ui->table->setItem(row, 1, new QTableWidgetItem(
                             tr("%1 bytes: ").arg(data.size()).toUtf8()
                             +data.mid(0, 4096)));
    }
  }
  ui->table->resizeColumnToContents(1);
  ui->table->resizeColumnToContents(0);
}

void ClipboardInspectorForm::clear_clipboard() {
  _clipboard->clear();
}
