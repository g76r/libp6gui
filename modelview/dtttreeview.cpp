#include "dtttreeview.h"
#include "dtt/documentmanager.h"
#include "modelview/shareduiitem.h"
#include <QtDebug>

DttTreeView::DttTreeView(QWidget *parent) : EnhancedTreeView(parent) {
  connect(this, SIGNAL(entered(QModelIndex)),
          this, SLOT(itemHovered(QModelIndex)));
  connect(this, SIGNAL(viewportEntered()), this, SLOT(setMouseoverTarget()));
  connect(this, SIGNAL(leaved()), this, SLOT(clearMouseoverTarget()));
}

void DttTreeView::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DttTreeView::setModel(QAbstractItemModel *model) {
  EnhancedTreeView::setModel(model);
  // LATER should clear _selectedItemsIds ?
  setMouseTracking(model);
  if (underMouse())
    setMouseoverTarget();
}

void DttTreeView::itemHovered(const QModelIndex &index) {
  QAbstractItemModel *m = model();
  QString id;
  if (m) {
    _mousePosition = index;
    id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
  }
  setMouseoverTarget(id);
}

void DttTreeView::setMouseoverTarget(QString itemId)  {
  if (itemId.isNull())
    _mousePosition = QModelIndex();
  TargetManager *tm = targetManager();
  if (tm) {
    QList<QString> itemIds;
    if (!itemId.isNull())
      itemIds.append(itemId);
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, itemIds);
  }
}

void DttTreeView::clearMouseoverTarget() {
  _mousePosition = QModelIndex();
  TargetManager *tm = targetManager();
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}

void DttTreeView::selectionChanged(const QItemSelection &selected,
                                   const QItemSelection &deselected) {
  EnhancedTreeView::selectionChanged(selected, deselected);
  QAbstractItemModel *m = model();
  if (m) {
    foreach(const QModelIndex index, deselected.indexes()) {
      if (index.column() == 0) {
        QString id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
        if (!id.isEmpty())
          _selectedItemsIds.removeAll(id);
      }
    }
    foreach(const QModelIndex index, selected.indexes()) {
      if (index.column() == 0) {
        QString id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
        if (!id.isEmpty())
          _selectedItemsIds.append(id);
      }
    }
  }
  emit selectedItemsChanged(_selectedItemsIds);
  if (hasFocus())
    setPrimaryTargetToSelection();
}

void DttTreeView::setPrimaryTargetToSelection() {
  TargetManager *tm = targetManager();
  if (tm)
    tm->setTarget(_perspectiveWidget, _selectedItemsIds);
}

void DttTreeView::focusInEvent(QFocusEvent *event) {
  EnhancedTreeView::focusInEvent(event);
  setPrimaryTargetToSelection();
}

void DttTreeView::focusOutEvent(QFocusEvent *event) {
  EnhancedTreeView::focusOutEvent(event);
  TargetManager *tm = targetManager();
  if (tm)
    tm->setTarget();
}

QString DttTreeView::mouseoverItemId() const {
  QAbstractItemModel *m = model();
  return _mousePosition.isValid() && m
      ? m->data(_mousePosition, SharedUiItem::QualifiedIdRole).toString()
      : QString();
}

TargetManager *DttTreeView::targetManager() const {
  if (_perspectiveWidget) {
    QPointer<DocumentManager> dm = _perspectiveWidget->documentManager();
    if (dm)
      return dm->targetManager();
  }
  return 0;
}
