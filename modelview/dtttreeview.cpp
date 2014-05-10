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

void DttTreeView::setDocumentManager(DocumentManager *dm) {
  _documentManager = dm;
}

DocumentManager *DttTreeView::documentManager() const {
  return _documentManager.data();
}

void DttTreeView::itemHovered(const QModelIndex &index) {
  QAbstractItemModel *m = model();
  QString id;
  if (m) {
    _mousePosition = index;
    id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
  }
  qDebug() << "GroupsTasksTreeView::itemHovered" << id << index.row()
           << index.column();
  setMouseoverTarget(id);
}

void DttTreeView::setMouseoverTarget(QString itemId)  {
  if (itemId.isNull())
    _mousePosition = QModelIndex();
  if (documentManager()) {
    QList<QString> itemIds;
    if (!itemId.isNull())
      itemIds.append(itemId);
    documentManager()->targetManager()->setTarget(
          TargetManager::MouseOverTarget, _perspectiveWidget, itemIds);
  }
}

void DttTreeView::clearMouseoverTarget() {
  _mousePosition = QModelIndex();
  if (documentManager())
    documentManager()->targetManager()
        ->setTarget(TargetManager::MouseOverTarget);
}

void DttTreeView::selectionChanged(const QItemSelection &selected,
                                           const QItemSelection &deselected) {
  EnhancedTreeView::selectionChanged(selected, deselected);
  setPrimaryTargetToSelection(); // LATER optimize
}

void DttTreeView::setPrimaryTargetToSelection() {
  QAbstractItemModel *m = model();
  qDebug() << "GroupsTasksTreeView::setPrimaryTargetToSelection" << documentManager() << m;
  if (documentManager() && m) {
    QStringList itemIds;
    foreach(const QModelIndex index, selectedIndexes()) {
      QString id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
      if (!id.isEmpty())
        itemIds.append(id);
    }
    qDebug() << "GroupsTasksTreeView::setPrimaryTargetToSelection ->" << itemIds;
    documentManager()->targetManager()->setTarget(_perspectiveWidget, itemIds);
  }
}

void DttTreeView::dragEnterEvent(QDragEnterEvent *event) {
  /*if (event->mimeData()->hasFormat(MIMETYPE_TOOL_ID)) {
    if (event->possibleActions() & Qt::MoveAction)
      event->setDropAction(Qt::MoveAction);
    event->accept();
  } else*/
    EnhancedTreeView::dragEnterEvent(event);
}

void DttTreeView::dragMoveEvent(QDragMoveEvent *event) {
  /*if (event->mimeData()->hasFormat(MIMETYPE_TOOL_ID))
    event->accept();
  else*/
    EnhancedTreeView::dragMoveEvent(event);
}

void DttTreeView::dropEvent(QDropEvent *event) {
  /*if (event->mimeData()->hasFormat(MIMETYPE_TOOL_ID)) {
    QPointer<Tool> tool;
    if (_perspectiveWidget && _perspectiveWidget->designDocumentManager()) {
      QString id = QString::fromUtf8(event->mimeData()->data(MIMETYPE_TOOL_ID));
      if (!id.isEmpty())
        tool = _perspectiveWidget->designDocumentManager()->toolById(id);
    }
    if (tool) {
      //qDebug() << "dropped" << event->possibleActions() << event->proposedAction();
      _mousePosition = indexAt(event->pos());
      // must set mouseover target now because hover* events of QGraphicsItems
      // are not received when draging and thus mouseover target has not been
      // updated when the drag was over the QGraphicsView
      _perspectiveWidget->designDocumentManager()->targetManager()
          ->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget);
      // TODO also set mouseover item for tools that apply to an item
      tool.data()->trigger();
      event->accept();
    } else
      event->ignore();
  } else if (false) {
    // FIXME internal move performing reparent
  } else*/
    EnhancedTreeView::dropEvent(event);
}

QString DttTreeView::mouseoverItemId() const {
  QAbstractItemModel *m = model();
  return _mousePosition.isValid() && m
      ? m->data(_mousePosition, SharedUiItem::QualifiedIdRole).toString()
      : QString();
}
