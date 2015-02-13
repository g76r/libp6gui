#include "dttgraphicsscene.h"
#include "dtt/perspectivewidget.h"
#include <QGraphicsView>
#include "dtt/targetmanager.h"
#include "dttgraphicsview.h"
#include <QGraphicsItem>
#include "modelview/shareduiitem.h"

DttGraphicsScene::DttGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  connect(this, SIGNAL(selectionChanged()),
          this, SLOT(propagateSelectionChanged()));
}

void DttGraphicsScene::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DttGraphicsScene::propagateSelectionChanged() {
  QStringList ids;
  foreach(QGraphicsItem *i, selectedItems()) {
    QString qualifiedId = i->data(SharedUiItem::QualifiedIdRole).toString();
    if (!qualifiedId.isEmpty()) {
      ids.append(qualifiedId);
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
