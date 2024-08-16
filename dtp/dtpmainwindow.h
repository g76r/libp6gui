/* Copyright 2014-2024 Hallowyn and others.
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
#ifndef DTPMAINWINDOW_H
#define DTPMAINWINDOW_H

#include "libp6gui_global.h"
#include "widget/enhancedmainwindow.h"

class DtpDocumentManager;

/** Document-Target-Perspective applications should have a special window which
 * closing triggers the application end. This window is the DtpMainWindow.
 * They must have only one DptMainWindow instance at a time, which is accessible
 * through DtpMainWindow::instance().
 *
 * When closing this window, document managers are asked to perform checks/save
 * operations if needed and all other top level windows are closed before this
 * one.
 *
 * Apart from dialogs, other top level widgets should be PerspectiveWidget that
 * have been popClone()'d.
 *
 * @see PerspectiveWidget
 */
class LIBP6GUISHARED_EXPORT DtpMainWindow : public EnhancedMainWindow {
  Q_OBJECT
  Q_DISABLE_COPY(DtpMainWindow)

private:
  QList<DtpDocumentManager*> _documentManagers; // LATER remove ?

public:
  explicit DtpMainWindow(QWidget *parent = 0);
  /** Syntaxic sugar. @see addDocumentManager(). */
  explicit DtpMainWindow(DtpDocumentManager *documentManager,
                         QWidget *parent = 0);
  ~DtpMainWindow();
  virtual void windowFocused();
  QList<DtpDocumentManager*> documentManagers() const;
  void addDocumentManager(DtpDocumentManager *documentManager);
  static DtpMainWindow *instance();
  /** If item found: ensure visible and focus it. If item writeable: start
    * editing it.
    * Default: call recursively PerspectiveWidget::startItemEdition() among
    * PerspectiveWidget children if any. */
  virtual bool startItemEdition(QString qualifiedId);

protected:
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;

private:
  void focusChanged(QWidget *oldWidget, QWidget *newWidget);
  void screenChanged(QScreen *screen);
};

#endif // DTPMAINWINDOW_H
