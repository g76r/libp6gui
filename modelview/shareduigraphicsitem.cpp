#include "shareduigraphicsitem.h"
#include "dttgraphicsscene.h"

SharedUiGraphicsItem::SharedUiGraphicsItem(QGraphicsItem *parent)
  : QGraphicsObject(parent) {
  setAcceptHoverEvents(true);
}

void SharedUiGraphicsItem::setUiItem(SharedUiItem uiItem) {
  prepareGeometryChange();
  _uiItem = uiItem;
}

void SharedUiGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  QGraphicsObject::hoverEnterEvent(event);
  DttGraphicsScene *dgs = qobject_cast<DttGraphicsScene*>(scene());
  if (dgs)
    dgs->setMouseOverItem(qualifiedId());
}

void SharedUiGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  QGraphicsObject::hoverLeaveEvent(event);
  DttGraphicsScene *dgs = qobject_cast<DttGraphicsScene*>(scene());
  if (dgs)
    dgs->clearMouseOverItem();
}
