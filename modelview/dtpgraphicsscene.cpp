#include "dtpgraphicsscene.h"
#include "dtp/perspectivewidget.h"
#include <QGraphicsView>
#include "dtp/targetmanager.h"
#include "dtpgraphicsview.h"
#include <QGraphicsItem>
#include "modelview/shareduiitem.h"

DtpGraphicsScene::DtpGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  connect(this, SIGNAL(selectionChanged()),
          this, SLOT(propagateSelectionChanged()));
}

void DtpGraphicsScene::setPerspectiveWidget(PerspectiveWidget *widget) {
  _perspectiveWidget = widget;
}

void DtpGraphicsScene::propagateSelectionChanged() {
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
        DtpGraphicsView *dgv = qobject_cast<DtpGraphicsView*>(gv);
        if (dgv)
          emit dgv->selectedItemsChanged(ids);
        break;
      }
  }
}

void DtpGraphicsScene::setMouseOverItem(QStringList ids) {
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
