#include "enhancedtreeview.h"

EnhancedTreeView::EnhancedTreeView(QWidget *parent) : QTreeView(parent) {
}

void EnhancedTreeView::leaveEvent(QEvent *event) {
  QTreeView::leaveEvent(event);
  emit leaved();
}
