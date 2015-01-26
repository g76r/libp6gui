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
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    QStringList itemIds;
    if (!itemId.isNull())
      itemIds.append(itemId);
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, itemIds);
  }
}

void DttTreeView::clearMouseoverTarget() {
  _mousePosition = QModelIndex();
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget(TargetManager::MouseOverTarget);
}

void DttTreeView::selectionChanged(const QItemSelection &selected,
                                   const QItemSelection &deselected) {
  EnhancedTreeView::selectionChanged(selected, deselected);
  QAbstractItemModel *m = model();
  // LATER use a better way to identify rows than testing column 0, since this only works with selectionBehavior() == SelectRows
  if (m) {
    foreach(const QModelIndex &index, deselected.indexes()) {
      if (index.column() == 0) {
        _selectedItemsIndexes.removeAll(index);
      }
    }
    foreach(const QModelIndex &index, selected.indexes()) {
      if (index.column() == 0 && !_selectedItemsIndexes.contains(index)) {
        _selectedItemsIndexes.append(index);
      }
    }
  }
  QStringList selectedItemsIds = buildSelectedItemsIdsList();
  emit selectedItemsChanged(selectedItemsIds);
  if (hasFocus()) {
    TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
    if (tm)
      tm->setTarget(_perspectiveWidget, selectedItemsIds);
  }
}

void DttTreeView::focusInEvent(QFocusEvent *event) {
  EnhancedTreeView::focusInEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    tm->setTarget(_perspectiveWidget, buildSelectedItemsIdsList());
    // set mouseover target in case overed item's id has just changed (which is
    // the case when focus in event is received just after an item id widget
    // editor has been closed)
    QString id = mouseoverItemId();
    tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, id);
  }
}

QStringList DttTreeView::buildSelectedItemsIdsList() const {
  // the purpose of this method is to transform selection indexes into item ids
  // ids cannot be kept as is because in case an item id changes, selection does
  // not change in the indexes point of view and target becomes wrong
  // LATER optimize for large selections, keeping ids list and subscribing to DocumentManager::itemChanged() to handle ids changes rather than rebuilding the whole list every time
  QStringList selectedItemsIds;
  selectedItemsIds.clear();
  foreach (const QModelIndex &index, _selectedItemsIndexes) {
    QString id = index.data(SharedUiItem::QualifiedIdRole).toString();
    if (!id.isEmpty())
      selectedItemsIds.append(id);
  }
  return selectedItemsIds;
}

void DttTreeView::focusOutEvent(QFocusEvent *event) {
  EnhancedTreeView::focusOutEvent(event);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm)
    tm->setTarget();
}

QString DttTreeView::mouseoverItemId() const {
  QAbstractItemModel *m = model();
  return _mousePosition.isValid() && m
      ? m->data(_mousePosition, SharedUiItem::QualifiedIdRole).toString()
      : QString();
}
