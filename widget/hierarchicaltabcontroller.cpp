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
#include "hierarchicaltabcontroller.h"
#include <QtDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QLayout>

#define CELL_WIDTH 80
#define CELL_HEIGHT 16

// TODO utiliser QPixmapCache
// http://qt-quarterly.developpez.com/qq-12/qpixmapcache/

QAtomicInt HierarchicalTabControllerItem::_counter(1); // must never be 0

HierarchicalTabController::HierarchicalTabController(QWidget *parent)
  : QWidget(parent), _structureHasChanged(false) {
}

int HierarchicalTabController::addItem(QString label, int parentId,
                                       void *pointer) {
  HierarchicalTabControllerItem parent = _items.value(parentId);
  HierarchicalTabControllerItem item(label, parent.id(), pointer);
  //qDebug() << "addItem" << label << item.id() << parentId << parent.id();
  return addItem(item);
}

int HierarchicalTabController::addItem(QString label, void *parentPointer,
                                       void *pointer) {
  return addItem(label, _pointers.value(parentPointer, 0), pointer);
}

int HierarchicalTabController::addItem(HierarchicalTabControllerItem item) {
  //qDebug() << "addItem" << item.label() << item.id() << item.parentId();
  _items.insert(item.id(), item);
  if (!item.parentId())
    _roots.append(item.id());
  else
    _edges.insert(item.parentId(), item.id());
  _structureHasChanged = true;
  if (item.pointer())
    _pointers.insert(item.pointer(), item.id());
  updateGeometry();
  return item.id();
}

void HierarchicalTabController::computeStructure(int id, int x, int y) const {
  HierarchicalTabControllerItem &item = _items[id];
  if (item.isNull())
    return;
  int width = 0, depth = 1, eldersWidth = 0;
  QList<int> children;
  foreach (int childId, _edges.values(id))
    children.prepend(childId);
  foreach (int childId, children) {
    computeStructure(childId, x+eldersWidth, y+1);
    HierarchicalTabControllerItem &child = _items[childId];
    if (child.isNull())
      continue; // never happens
    if (child._depth >= depth)
      depth = child._depth+1;
    width += child._width;
    eldersWidth += child._width;
  }
  if (!width)
    width = 1;
  item._depth = depth;
  item._width = width;
  item._x = x;
  item._y = y;
  for (int i = x; i < x+width; ++i)
    _matrix.insert(QPoint(i, y), id);
  //qDebug() << "compute" << id << item._x << item._y << item.parentId()
  //         << item._width << item._depth;
}

void HierarchicalTabController::computeStructure() const {
  if (!_structureHasChanged)
    return;
  _structureHasChanged = false;
  _matrix.clear();
  _width = _depth = 0;
  int eldersWidth = 0;
  foreach(int rootId, _roots) {
    computeStructure(rootId, eldersWidth, 0);
    HierarchicalTabControllerItem root = _items.value(rootId);
    //qDebug() << "foo" << root._x << root._y << root._width << root._depth;
    _width += root._width;
    eldersWidth += root._width;
    if (root._depth > _depth)
      _depth = root._depth;
  }
  //qDebug() << "compute all" << _width << _depth;
  //qDebug() << _matrix;
  if (_selection.isEmpty() && !_roots.isEmpty()) {
    int id = _roots.first();
    while (!_edges.values(id).isEmpty()) {
      id = _edges.values(id).last();
    }
    HierarchicalTabControllerItem first = _items.value(id);
    _selection.append(id);
    emit selected(id);
    emit selected(first.pointer());
    emit selected(first.label());
  }
}

int HierarchicalTabController::itemDepth() const {
  if (_structureHasChanged)
    computeStructure();
  return _depth;
}

int HierarchicalTabController::itemTreeWidth() const {
  if (_structureHasChanged)
    computeStructure();
  return _width;
}

QSize HierarchicalTabController::sizeHint() const {
  if (_structureHasChanged)
    computeStructure();
  QSize s(itemTreeWidth()*CELL_WIDTH, itemDepth()*CELL_HEIGHT);
  //qDebug() << "HierarchicalTabController::sizeHint" << s << itemTreeWidth() << itemDepth() << _items.size();
  return s;
}

QSize HierarchicalTabController::minimumSizeHint() const {
  return sizeHint();
}

void HierarchicalTabController::paintCell(QPainter &p, int id) {
  // LATER support other positions than south
  // LATER antialias, draw lights, shadows and so on
  // TODO support palette roles
  HierarchicalTabControllerItem i = _items.value(id);
  QRect r(i._x*CELL_WIDTH, rect().height()-CELL_HEIGHT*(i._y+1),
          CELL_WIDTH*i._width, CELL_HEIGHT-1);
  QPainterPath pp;
  if (_selection.contains(id)) {
    pp.moveTo(r.topRight());
    pp.lineTo(r.bottomRight()+QPoint(-6,0));
    pp.lineTo(r.bottomLeft()+QPoint(6,0));
    pp.lineTo(r.topLeft());
    pp.closeSubpath();
    p.fillPath(pp, Qt::white);
    pp = QPainterPath();
  }
  pp.moveTo(r.right(), 0);
  pp.lineTo(r.topRight()+QPoint(-1,0));
  pp.lineTo(r.bottomRight()+QPoint(-6,0));
  pp.lineTo(r.bottomLeft()+QPoint(6,0));
  pp.lineTo(r.topLeft()+QPoint(1,0));
  pp.lineTo(r.left(), 0);
  p.drawPath(pp);
  p.setPen(Qt::black);
  p.drawText(r.adjusted(6, 1, -6, -1), Qt::AlignHCenter|Qt::AlignVCenter,
             i.label());
}

void HierarchicalTabController::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);
  if (_structureHasChanged)
    computeStructure();
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  //p.setRenderHint(QPainter::TextAntialiasing);
  foreach (HierarchicalTabControllerItem item, _items)
    paintCell(p, item.id());
  p.drawLine(0, 0, itemTreeWidth()*CELL_WIDTH, 0);
  p.end();
}

HierarchicalTabControllerItem HierarchicalTabController
::itemUnderMouse(QPoint mousePos) const {
  if (_structureHasChanged)
    computeStructure();
  QPoint pos(mousePos.x()/CELL_WIDTH, _depth-(mousePos.y()/CELL_HEIGHT)-1);
  return _items.value(_matrix.value(pos));
}

void HierarchicalTabController::mouseReleaseEvent(QMouseEvent *e) {
  select(itemUnderMouse(e->pos()).id());
}

void HierarchicalTabController::select(int id) {
  HierarchicalTabControllerItem i = _items.value(id);
  if (i.isNull())
    return; // e.g. clicked outside of any tab
  if (_selection.contains(i.id()) && _selection.size() == 1)
    return; // already selected
  unselectAll();
  _selection.append(i.id());
  emit selected(i.id());
  emit selected(i.pointer());
  emit selected(i.label());
  update();
}

void HierarchicalTabController::select(void *pointer) {
  select(_pointers.value(pointer));
}

void HierarchicalTabController::unselectAll() {
  foreach (int id, _selection) {
    HierarchicalTabControllerItem i = _items.value(id);
    if (!i.isNull()) {
      emit unselected(id);
      emit unselected(i.pointer());
      emit unselected(i.label());
    }
  }
  _selection.clear();
  update();
}

void HierarchicalTabController::mouseDoubleClickEvent(QMouseEvent *e) {
  HierarchicalTabControllerItem i = itemUnderMouse(e->pos());
  //qDebug() << "mouseDoubleClick" << i.label();
  if (i.isNull())
    return; // clicked outside of any tab
  emit activated(i.id());
  emit activated(i.pointer());
  emit activated(i.label());
}
