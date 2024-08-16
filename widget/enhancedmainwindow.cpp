/* Copyright 2024 Hallowyn, Gregoire Barbier and others.
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
#include "enhancedmainwindow.h"
#include <QApplication>
#include <QLabel>

EnhancedMainWindow::EnhancedMainWindow(QWidget *parent)
  : QMainWindow(parent) {

}

void EnhancedMainWindow::closeEvent(QCloseEvent *event) {
  // closing subwindows before main window is needed because subwindows may
  // rely on resources allocated by main window, such as the databases,
  // some QAbstractItemModels, loaded configuration, etc.
  for (QWidget *w: QApplication::topLevelWidgets()) {
    if (w != this)
      w->close();
  }
  // creating a label here is a hack to ensure that the main window and its
  // chidren destructors will all be called, even on platforms (at less Windows)
  // where closing the last window kills the program
  QLabel *label = new QLabel(tr("Shuting down..."));
  label->setAttribute(Qt::WA_DeleteOnClose);
  label->setAttribute(Qt::WA_QuitOnClose);
  label->setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
  label->setMargin(20);
  label->move(x()+width()/2, y()+height()/2);
  label->show();
  QMetaObject::invokeMethod(label, "deleteLater", Qt::QueuedConnection);
  QMainWindow::closeEvent(event);
}
