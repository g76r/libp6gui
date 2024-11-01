/* Copyright 2014-2024 Gregoire Barbier and others.
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
class QLineEdit;

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
  inline HierarchicalTabControllerItem()
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
  Q_PROPERTY(bool allowTabRename READ allowTabRename WRITE setAllowTabRename)

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
  _underlineSelected = false, _allowTabRename = true;
  int _mouseover_item = 0;
  QLineEdit *_editor;

public:
  explicit HierarchicalTabController(QWidget *parent = 0);
  int addItem(QString label, int parentId = 0, void *pointer = 0);
  int addItem(QString label, void *parentPointer = 0, void *pointer = 0);
  int addItem(HierarchicalTabControllerItem item);
  void setItemLabel(int id, const QString &label);
  void setItemLabel(void *pointer, const QString &label) {
    setItemLabel(item(pointer)._id, label);
  }
  void removeItem(int id);
  void removeItem(void *pointer) {
    removeItem(item(pointer)._id);
  }
  inline int itemCount() const { return _items.size(); }
  int itemDepth() const;
  int itemTreeWidth() const;
  QSize sizeHint() const override;
  QSize minimumSizeHint() const override;
  /** Returned item.isNull() if id not found.
    */
  HierarchicalTabControllerItem item(int id) const {
    return _items.value(id);
  }
  HierarchicalTabControllerItem item(void *pointer) const {
    for (auto [_,v]: _items.asKeyValueRange())
      if (pointer == v._pointer)
        return v;
    return {};
  }
  bool invertBgColor() const { return _invertBgColor; }
  void setInvertBgColor(bool invertBgColor) { _invertBgColor = invertBgColor; }
  bool drawBaseline() const { return _drawBaseline; }
  void setDrawBaseline(bool drawBaseline) { _drawBaseline = drawBaseline; }
  bool underlineSelected() const { return _underlineSelected; }
  void setUnderlineSelected(bool underlineSelected) {
    _underlineSelected = underlineSelected; }
  bool allowTabRename() const { return _allowTabRename; }
  void setAllowTabRename(bool allowTabRename) {
    _allowTabRename = allowTabRename; }

signals:
  void selected(int id, void *pointer, const QString &label) const;
  void unselected(int id, void *pointer, const QString &label);
  void removed(int id, void *pointer, const QString &label);
  /** emited when a tab label is double-clicked
    * requires allowTabRename property set to false */
  void activated(int id, void *pointer, const QString &label);
  /** emited when a tab is hovered or stop being hovered (with {} values)
   *  needs enabled mouse tracking to work */
  void hovered(int id, void *pointer, const QString &label);
  /** emited when a tab label is changed interactively (after double click edit)
    * requires allowTabRename property set to true */
  void renamed(int id, void *pointer, const QString &label);

public slots:
  void select(int id);
  void select(void *pointer);
  void unselectAll();

protected:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent *) override;
  void enterEvent(QEnterEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void leaveEvent(QEvent *) override;
  HierarchicalTabControllerItem itemAt(const QPoint &mousePos) const;

private:
  void computeStructure(int id, int x, int y) const;
  void computeStructure() const;
  void paintCell(QPainter &p, int id);
  QRect item_rect(const HierarchicalTabControllerItem &i);
  void commitEdition();
  void cancelEdition();
};

#endif // HIERARCHICALTABCONTROLLER_H
