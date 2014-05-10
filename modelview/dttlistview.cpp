#include "dttlistview.h"
#include "dtt/documentmanager.h"
#include "modelview/shareduiitem.h"
#include <QtDebug>

DttListView::DttListView(QWidget *parent) : EnhancedListView(parent) {
  connect(this, SIGNAL(entered(QModelIndex)),
          this, SLOT(itemHovered(QModelIndex)));
  connect(this, SIGNAL(viewportEntered()), this, SLOT(setMouseoverTarget()));
  connect(this, SIGNAL(leaved()), this, SLOT(clearMouseoverTarget()));
}

void DttListView::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DttListView::setModel(QAbstractItemModel *model) {
  EnhancedListView::setModel(model);
  setMouseTracking(model);
  if (underMouse())
    setMouseoverTarget();
}

void DttListView::setDocumentManager(DocumentManager *dm) {
  _documentManager = dm;
}

DocumentManager *DttListView::documentManager() const {
  return _documentManager.data();
}

void DttListView::itemHovered(const QModelIndex &index) {
  QAbstractItemModel *m = model();
  QString id;
  if (m) {
    _mousePosition = index;
    id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
  }
  setMouseoverTarget(id);
}

void DttListView::setMouseoverTarget(QString itemId)  {
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

void DttListView::clearMouseoverTarget() {
  _mousePosition = QModelIndex();
  if (documentManager())
    documentManager()->targetManager()
        ->setTarget(TargetManager::MouseOverTarget);
}

void DttListView::selectionChanged(const QItemSelection &selected,
                                           const QItemSelection &deselected) {
  EnhancedListView::selectionChanged(selected, deselected);
  setPrimaryTargetToSelection(); // LATER optimize
}

void DttListView::setPrimaryTargetToSelection() {
  QAbstractItemModel *m = model();
  if (documentManager() && m) {
    QStringList itemIds;
    foreach(const QModelIndex index, selectedIndexes()) {
      QString id = m->data(index, SharedUiItem::QualifiedIdRole).toString();
      if (!id.isEmpty())
        itemIds.append(id);
    }
    documentManager()->targetManager()->setTarget(_perspectiveWidget, itemIds);
  }
}

void DttListView::dragEnterEvent(QDragEnterEvent *event) {
  /*if (event->mimeData()->hasFormat(MIMETYPE_TOOL_ID)) {
    if (event->possibleActions() & Qt::MoveAction)
      event->setDropAction(Qt::MoveAction);
    event->accept();
  } else*/
    EnhancedListView::dragEnterEvent(event);
}

void DttListView::dragMoveEvent(QDragMoveEvent *event) {
  /*if (event->mimeData()->hasFormat(MIMETYPE_TOOL_ID))
    event->accept();
  else*/
    EnhancedListView::dragMoveEvent(event);
}

void DttListView::dropEvent(QDropEvent *event) {
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
    EnhancedListView::dropEvent(event);
}

QString DttListView::mouseoverItemId() const {
  QAbstractItemModel *m = model();
  return _mousePosition.isValid() && m
      ? m->data(_mousePosition, SharedUiItem::QualifiedIdRole).toString()
      : QString();
}
