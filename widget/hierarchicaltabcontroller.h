/* Copyright 2014-2023 Gregoire Barbier and others.
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
#ifndef HIERARCHICALTABCONTROLLER_H
#define HIERARCHICALTABCONTROLLER_H

#include "libp6gui_global.h"
#include <QWidget>

class QPainter;
class HierarchicalTabController;

// TODO permit multiselection (e.g. to delete several tabs at once)
// TODO rename widget
// TODO implement QAbstractItemView and use a DesignTreeModel model on document

/*

recoit un arbre de qstring,void* ou de qstring,qbrush,qpen,qfont,void*
recoit des modifs d'arbre
signale select/unselect

propriete: select=none,one,zero-or-one,on-or-many,any; position=north,south,east,west

  */

inline uint qHash(QPoint p) {
  return p.x()+(p.y()<<16);
}

class LIBP6GUISHARED_EXPORT HierarchicalTabControllerItem {
  // TODO implicit sharing
  friend class HierarchicalTabController;
private:
  static QAtomicInt _counter;
  int _id, _parentId;
  void *_pointer;
  QString _label;
  int _x, _y, _width, _depth;

public:
  explicit inline HierarchicalTabControllerItem(
      QString label, int parentId = 0, void *pointer = 0)
    : _parentId(parentId), _pointer(pointer), _label(label),
      _x(0), _y(0), _width(0), _depth(0) {
    _id = _counter.fetchAndAddRelaxed(1);
  }
  explicit inline HierarchicalTabControllerItem()
    : _id(0), _parentId(0), _pointer(0),
      _x(0), _y(0), _width(0), _depth(0) {
  }
  inline HierarchicalTabControllerItem(const HierarchicalTabControllerItem &o)
    : _id(o._id), _parentId(o._parentId), _pointer(o._pointer), _label(o._label),
      _x(o._x), _y(o._y), _width(o._width),
      _depth(o._depth) {
  }
  inline int id() const { return _id; }
  /** @return 0 means orphan
    */
  inline int parentId() const { return _parentId; }
  inline void *pointer() const { return _pointer; }
  inline QString label() const { return _label; }
  inline bool isNull() const { return !_id; }
};

class LIBP6GUISHARED_EXPORT HierarchicalTabController : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool invertBgColor READ invertBgColor WRITE setInvertBgColor)
  Q_PROPERTY(bool drawBaseline READ drawBaseline WRITE setDrawBaseline)
  Q_PROPERTY(bool underlineSelected READ underlineSelected
             WRITE setUnderlineSelected)
private:
  mutable QHash<int,HierarchicalTabControllerItem> _items;
  QList<int> _roots;
  QMultiMap<int,int> _edges;
  mutable bool _structureHasChanged;
  mutable QHash<QPoint,int> _matrix; // converts width×depth coordinates to id
  mutable int _depth, _width;
  mutable QList<int> _selection;
  QMap<void*,int> _pointers;
  bool _invertBgColor = false, _drawBaseline = false,
  _underlineSelected = false;

public:
  explicit HierarchicalTabController(QWidget *parent = 0);
  int addItem(QString label, int parentId = 0, void *pointer = 0);
  int addItem(QString label, void *parentPointer = 0, void *pointer = 0);
  int addItem(HierarchicalTabControllerItem item);
  // TODO removeItem (recursively or not)
  // TODO insertItem
  inline int itemCount() const { return _items.size(); }
  int itemDepth() const;
  int itemTreeWidth() const;
  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;
  void paintEvent(QPaintEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  //void wheelEvent(QWheelEvent *);
  /** Returned item.isNull() if id not found.
    */
  HierarchicalTabControllerItem item(int id) const {
    return _items.value(id);
  }
  bool invertBgColor() const { return _invertBgColor; }
  void setInvertBgColor(bool invertBgColor) { _invertBgColor = invertBgColor; }
  bool drawBaseline() const { return _drawBaseline; }
  void setDrawBaseline(bool drawBaseline) { _drawBaseline = drawBaseline; }
  bool underlineSelected() const { return _underlineSelected; }
  void setUnderlineSelected(bool underlineSelected) {
    _underlineSelected = underlineSelected; }

signals:
  void selected(int id) const;
  void selected(void *pointer) const;
  void selected(QString label) const;
  void unselected(int id);
  void unselected(void *pointer);
  void unselected(QString label);
  void activated(int id);
  void activated(void *pointer);
  void activated(QString label);

public slots:
  void select(int id);
  void select(void *pointer);
  void unselectAll();

private:
  void computeStructure(int id, int x, int y) const;
  void computeStructure() const;
  void paintCell(QPainter &p, int id);
  HierarchicalTabControllerItem itemUnderMouse(QPoint mousePos) const;
};

#endif // HIERARCHICALTABCONTROLLER_H
