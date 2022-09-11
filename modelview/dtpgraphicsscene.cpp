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
#include "dtpgraphicsscene.h"
#include "dtp/perspectivewidget.h"
#include <QGraphicsView>
#include "dtp/targetmanager.h"
#include "dtpgraphicsview.h"
#include <QGraphicsItem>
#include "modelview/shareduiitem.h"

DtpGraphicsScene::DtpGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  connect(this, SIGNAL(selectionChanged()),
          this, SLOT(propagateSelectionChanged()));
}

void DtpGraphicsScene::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DtpGraphicsScene::propagateSelectionChanged() {
  QStringList ids;
  foreach(QGraphicsItem *i, selectedItems()) {
    QString qualifiedId = i->data(SharedUiItem::QualifiedIdRole).toString();
    if (!qualifiedId.isEmpty()) {
      ids.append(qualifiedId);
    }
  }
  _selectedItemsIds = ids;
  emit selectedItemsChanged(ids);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        tm->setTarget(_perspectiveWidget, ids);
        DtpGraphicsView *dgv = qobject_cast<DtpGraphicsView*>(gv);
        if (dgv)
          emit dgv->selectedItemsChanged(ids);
        break;
      }
  }
}

void DtpGraphicsScene::setMouseOverItem(QStringList ids) {
  _mouseoverItemsIds = ids;
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, ids);
        break;
      }
  }
}
