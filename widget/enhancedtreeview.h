#ifndef ENHANCEDTREEVIEW_H
#define ENHANCEDTREEVIEW_H

#include <QTreeView>
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT EnhancedTreeView : public QTreeView {
  Q_OBJECT
public:
  explicit EnhancedTreeView(QWidget *parent = 0);
  void leaveEvent(QEvent *event);

signals:
  void leaved();
};

#endif // ENHANCEDTREEVIEW_H
