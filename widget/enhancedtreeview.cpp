#include "enhancedtreeview.h"
#include "dtt/perspectivewidget.h"
#include "dtt/mainwindow.h"
#include <QKeyEvent>

EnhancedTreeView::EnhancedTreeView(QWidget *parent) : QTreeView(parent),
  _ignoreKeyboardInput(false) {
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
