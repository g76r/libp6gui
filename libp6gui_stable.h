/* Copyright 2023-2024 Gregoire Barbier and others.
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
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBP6GUI_STABLE_H
#define LIBP6GUI_STABLE_H

// C

#if defined __cplusplus

// C++

// Qt
#include <QAbstractButton>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QAbstractSpinBox>
#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QCheckBox>
#include <QClipboard>
#include <QColor>
#include <QComboBox>
#include <QDrag>
#include <QDropEvent>
#include <QEvent>
#include <QGraphicsItem>
#include <QGraphicsLayout>
#include <QGraphicsLayoutItem>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPen>
#include <QPersistentModelIndex>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPropertyAnimation>
#include <QRect>
#include <QRectF>
#include <QScreen>
#include <QScrollBar>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QStyleOptionComboBox>
#include <QStyleOptionGraphicsItem>
#include <QStylePainter>
#include <QTableView>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolBox>
#include <QToolButton>
#include <QTreeView>
#include <QUndoStack>
#include <QWheelEvent>
#include <QWidget>
#include <QWindow>
#include <QWindowStateChangeEvent>

// libp6core
#include "modelview/shareduiitemsmodel.h"
#include "modelview/shareduiitemsmatrixmodel.h"
#include "modelview/inmemoryshareduiitemdocumentmanager.h"
#include "modelview/stringlistdiffmodel.h"
#include "util/utf8stringlist.h"
#include "modelview/paramsetmodel.h"
#include "format/stringutils.h"

#endif // __cplusplus

#endif // LIBP6GUI_STABLE_H
