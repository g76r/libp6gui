#include "collapsiblegroupbox.h"

CollapsibleGroupBox::CollapsibleGroupBox(QWidget *parent)
  : QGroupBox(parent) {
}

void CollapsibleGroupBox::doCollapse(bool collapsing) {
  // LATER handle animation
  foreach (QObject *o, children()) {
    QWidget *w = qobject_cast<QWidget*>(o);
    if (w)
      w->setHidden(collapsing);
  }
  if (collapsing)
    emit collapsed();
  else
    emit expanded();
}
