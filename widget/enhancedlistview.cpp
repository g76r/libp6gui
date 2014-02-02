#include "enhancedlistview.h"
#include <QDropEvent>
#include <QKeyEvent>

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

void EnhancedListView::keyPressEvent(QKeyEvent *event) {
  if (_ignoreKeyboardInput)
    event->setAccepted(false);
  else
    QListView::keyPressEvent(event);
}
