#ifndef HIERARCHICALTABCONTROLLER_H
#define HIERARCHICALTABCONTROLLER_H

#include <QWidget>
#include <QAtomicInt>
#include <QBrush>
#include <QList>
#include <QHash>
#include <QMap>
#include "libh6ncsu_global.h"

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

class LIBH6NCSUSHARED_EXPORT HierarchicalTabControllerItem {
  // TODO implicit sharing
  friend class HierarchicalTabController;
private:
  static QAtomicInt _counter;
  int _id, _parentId;
  void *_pointer;
  QString _label;
  QBrush _normalBg;
  int _x, _y, _width, _depth;

public:
  explicit inline HierarchicalTabControllerItem(
      QString label, int parentId = 0, void *pointer = 0)
    : _parentId(parentId), _pointer(pointer), _label(label),
      _normalBg(Qt::gray) {
    _id = _counter.fetchAndAddRelaxed(1);
  }
  explicit inline HierarchicalTabControllerItem()
    : _id(0), _parentId(0), _pointer(0) {
  }
  inline HierarchicalTabControllerItem(const HierarchicalTabControllerItem &o)
    : _id(o._id), _parentId(o._parentId), _pointer(o._pointer), _label(o._label),
      _normalBg(o._normalBg), _x(o._x), _y(o._y), _width(o._width),
      _depth(o._depth) {
  }
  inline int id() const { return _id; }
  /** @return 0 means orphan
    */
  inline int parentId() const { return _parentId; }
  inline void *pointer() const { return _pointer; }
  inline QString label() const { return _label; }
  inline bool isNull() const { return !_id; }
  inline QBrush normalBg() const { return _normalBg; }
};

class LIBH6NCSUSHARED_EXPORT HierarchicalTabController : public QWidget {
  Q_OBJECT
private:
  mutable QHash<int,HierarchicalTabControllerItem> _items;
  QList<int> _roots;
  QMultiMap<int,int> _edges;
  mutable bool _structureHasChanged;
  mutable QHash<QPoint,int> _matrix; // converts width×depth coordinates to id
  mutable int _depth, _width;
  mutable QList<int> _selection;
  QMap<void*,int> _pointers;

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
  QSize sizeHint() const;
  QSize minimumSizeHint() const;
  void paintEvent(QPaintEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);
  //void wheelEvent(QWheelEvent *);
  /** Returned item.isNull() if id not found.
    */
  HierarchicalTabControllerItem item(int id) const {
    return _items.value(id);
  }

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
