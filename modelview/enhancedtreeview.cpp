#include "enhancedtreeview.h"
#include "dtt/perspectivewidget.h"
#include "dtt/mainwindow.h"
#include <QKeyEvent>

EnhancedTreeView::EnhancedTreeView(QWidget *parent) : QTreeView(parent),
  _ignoreKeyboardInput(false), _expandToDepthOnChange(0) {
}

void EnhancedTreeView::leaveEvent(QEvent *event) {
  QTreeView::leaveEvent(event);
  emit leaved();
}

void EnhancedTreeView::keyPressEvent(QKeyEvent *event) {
  if (_ignoreKeyboardInput)
    event->setAccepted(false);
  else
    QTreeView::keyPressEvent(event);
}

void EnhancedTreeView::setModel(QAbstractItemModel *model) {
  QAbstractItemModel *old = this->model();
  if (old) {
    disconnect(model, SIGNAL(modelReset()),
               this, SLOT(rowsAppeared()));
    disconnect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
               this, SLOT(rowsAppeared()));
    disconnect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
               this, SLOT(rowsAppeared()));
  }
  QTreeView::setModel(model);
  if (model) {
    connect(model, SIGNAL(modelReset()),
            this, SLOT(rowsAppeared()));
    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(rowsAppeared()));
    connect(model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(rowsAppeared()));
    rowsAppeared();
  }
}

void EnhancedTreeView::rowsAppeared() {
  if (_expandToDepthOnChange >= 0)
    expandToDepth(_expandToDepthOnChange);
  else
    expandAll();
}
