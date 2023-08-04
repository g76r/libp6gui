/* Copyright 2015-2023 Gregoire Barbier and others.
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
#ifndef ENHANCEDPLAINTEXTEDIT_H
#define ENHANCEDPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include "libp6gui_global.h"

/** Enhanced QPlainTextEdit
 *
 * Additionnal signals:
 * - editingFinished(): emited whenever enter/return is pressed or focus is lost
 *   (like QAbstractSpinBox::editingFinished()).
 */
class LIBP6GUISHARED_EXPORT EnhancedPlainTextEdit : public QPlainTextEdit {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedPlainTextEdit)

public:
  explicit EnhancedPlainTextEdit(QWidget *parent = 0)
    : QPlainTextEdit(parent) { }

signals:
  void editingFinished();

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
};

#endif // ENHANCEDPLAINTEXTEDIT_H
