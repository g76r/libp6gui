#ifndef ENHANCEDLISTVIEW_H
#define ENHANCEDLISTVIEW_H

#include <QListView>
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT EnhancedListView : public QListView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreInternalDrop READ ignoreInternalDrop
             WRITE setIgnoreInternalDrop)
private:
  bool _ignoreInternalDrop;

public:
  explicit EnhancedListView(QWidget *parent = 0);
  void dropEvent(QDropEvent *e);
  inline bool ignoreInternalDrop() const { return _ignoreInternalDrop; }
  inline void setIgnoreInternalDrop(bool ignoreInternalDrop) {
    _ignoreInternalDrop = ignoreInternalDrop;
  }
};

#endif // ENHANCEDLISTVIEW_H
