/* Copyright 2014-2015 Hallowyn and others.
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
#include "perspectivestackedwidget.h"
#include "dtpdocumentmanager.h"

PerspectiveStackedWidget::PerspectiveStackedWidget(PerspectiveWidget *parent)
  : QStackedWidget(parent), _perspectiveWidget(parent) {
}

void PerspectiveStackedWidget::enterEvent(QEvent *event) {
  QStackedWidget::enterEvent(event);
  setMouseoverTarget(true);
}

void PerspectiveStackedWidget::leaveEvent(QEvent *event) {
  QStackedWidget::leaveEvent(event);
  setMouseoverTarget(false);
}

void PerspectiveStackedWidget::perspectiveChanged() {
  if (underMouse())
    setMouseoverTarget(true);
}

void PerspectiveStackedWidget::setMouseoverTarget(bool set) {
  if (_perspectiveWidget) {
    DtpDocumentManager *manager = _perspectiveWidget->documentManager();
    if (manager) {
      if (set)
        manager->targetManager()->setTarget(TargetManager::MouseOverTarget,
                                            _perspectiveWidget);
      else
        manager->targetManager()->setTarget(TargetManager::MouseOverTarget,
                                            QPointer<PerspectiveWidget>());
    }
  }
}

