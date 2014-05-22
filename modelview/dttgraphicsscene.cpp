#include "dttgraphicsscene.h"
#include "dtt/perspectivewidget.h"
#include <QGraphicsView>
#include "dtt/targetmanager.h"

DttGraphicsScene::DttGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  connect(this, SIGNAL(selectionChanged()),
          this, SLOT(propagateSelectionChanged()));
}

void DttGraphicsScene::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DttGraphicsScene::addItem(SharedUiGraphicsItem *item) {
  QGraphicsScene::addItem(item);
  _sharedUiItems.append(item);
}

void DttGraphicsScene::removeItem(SharedUiGraphicsItem *item) {
  QGraphicsScene::removeItem(item);
  _sharedUiItems.removeAll(item);
}

void DttGraphicsScene::propagateSelectionChanged() {
  QStringList ids;
  foreach(QGraphicsItem *i, selectedItems()) {
    SharedUiGraphicsItem *sui = reinterpret_cast<SharedUiGraphicsItem*>(i);
    // LATER do the lookup in a fast collection such as QSet
    if (_sharedUiItems.contains(sui)) {
      ids.append(sui->id());
    }
  }
  _selectedItemsIds = ids;
  emit selectedItemsChanged(ids);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        PerspectiveWidget *pw = qobject_cast<PerspectiveWidget*>(gv);
        tm->setTarget(pw, ids);
        break;
      }
  }
}

void DttGraphicsScene::setMouseOverItem(QStringList ids) {
  _mouseoverItemsIds = ids;
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        PerspectiveWidget *pw = qobject_cast<PerspectiveWidget*>(gv);
        tm->setTarget(TargetManager::MouseOverTarget, pw, ids);
        break;
      }
  }
}
