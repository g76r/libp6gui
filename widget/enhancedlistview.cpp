#include "enhancedlistview.h"
#include <QDropEvent>
#include <QtDebug>

EnhancedListView::EnhancedListView(QWidget *parent)
  : QListView(parent), _ignoreInternalDrop(false) {
}

void EnhancedListView::dropEvent(QDropEvent *e) {
  if (ignoreInternalDrop() && e->source() == this) {
    e->ignore();
    if (viewport())
      viewport()->update();
  } else
    QListView::dropEvent(e);
}
