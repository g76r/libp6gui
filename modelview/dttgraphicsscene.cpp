#include "dttgraphicsscene.h"
#include "dtt/perspectivewidget.h"
#include <QGraphicsView>
#include "dtt/targetmanager.h"
#include "dttgraphicsview.h"

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
    SharedUiGraphicsItem *sui = static_cast<SharedUiGraphicsItem*>(i);
    // LATER do the lookup in a fast collection such as QSet
    if (_sharedUiItems.contains(sui)) {
      ids.append(sui->qualifiedId());
    }
  }
  _selectedItemsIds = ids;
  emit selectedItemsChanged(ids);
  TargetManager *tm = PerspectiveWidget::targetManager(_perspectiveWidget);
  if (tm) {
    foreach (QGraphicsView *gv, views())
      if (gv && gv->hasFocus()) {
        tm->setTarget(_perspectiveWidget, ids);
        DttGraphicsView *dgv = qobject_cast<DttGraphicsView*>(gv);
        if (dgv)
          emit dgv->selectedItemsChanged(ids);
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
        tm->setTarget(TargetManager::MouseOverTarget, _perspectiveWidget, ids);
        break;
      }
  }
}
