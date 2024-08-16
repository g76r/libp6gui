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
#include "dtpmainwindow.h"
#include "dtpdocumentmanager.h"
#include "dtpaction.h"
#include "widget/responsiveapplication.h"
#include <QLabel>
#include <QWindow>
#include <QShowEvent>
#include <QHideEvent>

static DtpMainWindow *singletonInstance = 0;

DtpMainWindow::DtpMainWindow(QWidget *parent) : EnhancedMainWindow(parent) {
  Q_ASSERT(singletonInstance == 0); // only one DtpMainWindow instance at a time
  singletonInstance = this;
  connect((QApplication*)QApplication::instance(), &QApplication::focusChanged,
          this, &DtpMainWindow::focusChanged);
}

DtpMainWindow::DtpMainWindow(DtpDocumentManager *documentManager,
                             QWidget *parent)
  : EnhancedMainWindow(parent) {
  addDocumentManager(documentManager);
}

DtpMainWindow::~DtpMainWindow() {
  singletonInstance = 0;
}

void DtpMainWindow::windowFocused() {
}

QList<DtpDocumentManager*> DtpMainWindow::documentManagers() const {
  return _documentManagers;
}

void DtpMainWindow::addDocumentManager(DtpDocumentManager *documentManager) {
  _documentManagers.append(documentManager);
}

DtpMainWindow *DtpMainWindow::instance() {
  return singletonInstance;
}

static bool startItemEditionAmongChildren(
    QString qualifiedId, QWidget *widget) {
  auto *pw = qobject_cast<PerspectiveWidget*>(widget);
  if (pw)
    return pw->startItemEdition(qualifiedId);
  for (QObject *o: widget->children()) {
    auto *widget = qobject_cast<QWidget*>(o);
    if (widget && startItemEditionAmongChildren(qualifiedId, widget))
      return true;
  }
  return false;
}

bool DtpMainWindow::startItemEdition(QString qualifiedId) {
  return startItemEditionAmongChildren(qualifiedId, this);
}

// This method set the perspective target when a widget get focus.
// For naive (non dtp-aware) widgets, ascendant widgets are searched until
// either a dtp-aware view or a PerspectiveWidget is found, e.g. when a plain
// QLineEdit is a child of a PerspectiveWidget form, when the QLineEdit get
// focused its parent will be targeted.
// In the other hand, dtp-aware widgets (e.g. DtpTreeView) expose properties
// that are used to set the new target (perspectiveWidget and primaryItemsIds).
void DtpMainWindow::focusChanged(QWidget *oldWidget, QWidget *newWidget) {
  //qDebug() << "DtpMainWindow::focusChanged" << oldWidget << newWidget;
  // reset target on old dm (needed if it was not the same than new one)
  DtpDocumentManager *oldDm = 0, *newDm = 0;
  while (oldWidget) {
    auto *pw =
        oldWidget->property("perspectiveWidget").value<PerspectiveWidget*>();
    //if (pw)
    //  qDebug() << "  old pw by prop" << oldWidget << pw;
    if (!pw) {
      pw = qobject_cast<PerspectiveWidget*>(oldWidget);
      //if (pw)
      //  qDebug() << "  old pw by cast" << oldWidget << pw;
    }
    if (pw) {
      oldDm = pw->documentManager();
      if (oldDm) {
        //qDebug() << "  found old pw" << pw << oldDm;
        break;
      }
    }
    oldWidget = oldWidget->parentWidget();
  }
  // set new widget target on new dm
  while (newWidget) {
    auto *pw =
        newWidget->property("perspectiveWidget").value<PerspectiveWidget*>();
    //if (pw)
    //  qDebug() << "  new pw by prop" << newWidget << pw;
    if (!pw) {
      pw = qobject_cast<PerspectiveWidget*>(newWidget);
      //if (pw)
      //  qDebug() << "  new pw by cast" << newWidget << pw;
    }
    if (pw) {
      newDm = pw->documentManager();
      if (newDm) {
        auto primaryItemsIds =
            newWidget->property("primaryItemsIds").value<QByteArrayList>();
        //qDebug() << "  found new pw" << pw << newDm << primaryItemsIds;
        newDm->targetManager()->setTarget(pw, primaryItemsIds);
        break;
      }
    }
    newWidget = newWidget->parentWidget();
  }
  if (oldDm && oldDm != newDm)
    oldDm->targetManager()->setTarget();
}

void DtpMainWindow::showEvent(QShowEvent *event) {
  QMainWindow::showEvent(event);
  connect(windowHandle(), &QWindow::screenChanged,
          this, &DtpMainWindow::screenChanged);
  // LATER also detect screen dpi change, in addition to screen change
  screenChanged(windowHandle()->screen());
}

void DtpMainWindow::hideEvent(QHideEvent *event) {
  disconnect(windowHandle(), &QWindow::screenChanged,
             this, &DtpMainWindow::screenChanged);
  QMainWindow::hideEvent(event);
}

void DtpMainWindow::screenChanged(QScreen *screen) {
  auto *app = qobject_cast<ResponsiveApplication*>(QApplication::instance());
  if (app)
    emit app->toplevelWidgetChangedScreen(this, screen);
}
