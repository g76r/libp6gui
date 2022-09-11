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
#ifndef DTPGRAPHICSVIEW_H
#define DTPGRAPHICSVIEW_H

#include "libp6gui_global.h"
#include "enhancedgraphicsview.h"
#include "dtp/perspectivewidget.h"

class LIBP6GUISHARED_EXPORT DtpGraphicsView : public EnhancedGraphicsView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsView)
  Q_PROPERTY(QStringList primaryItemsIds READ selectedItemsIds)
  PerspectiveWidget *_perspectiveWidget;

public:
  explicit DtpGraphicsView(QWidget *parent = 0);
  void setScene(QGraphicsScene *scene);
  QStringList selectedItemsIds() const;

signals:
  /** Same as DtpGraphicsScene::selectedItemsChanged() but only emited when
   * this view has focus */
  void selectedItemsChanged(QStringList selectedItemsIds);

protected:
  void focusInEvent(QFocusEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

#endif // DTPGRAPHICSVIEW_H
