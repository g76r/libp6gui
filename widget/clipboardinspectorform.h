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
#ifndef CLIPBOARDINSPECTORFORM_H
#define CLIPBOARDINSPECTORFORM_H

#include "libp6gui_global.h"
#include <QWidget>

class QClipboard;

namespace Ui {
class ClipboardInspectorForm;
}

class LIBP6GUISHARED_EXPORT ClipboardInspectorForm : public QWidget {
  Q_OBJECT
  Ui::ClipboardInspectorForm *ui;
  QClipboard *_clipboard = 0;

public:
  explicit ClipboardInspectorForm(QWidget *parent = nullptr);
  ~ClipboardInspectorForm();
  void update_display();
  void clear_clipboard();
};

#endif // CLIPBOARDINSPECTORFORM_H
