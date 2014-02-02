#ifndef ENHANCEDTREEVIEW_H
#define ENHANCEDTREEVIEW_H

#include <QTreeView>
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT EnhancedTreeView : public QTreeView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreKeyboardInput READ ignoreKeyboardInput
             WRITE setIgnoreKeyboardInput)
  bool _ignoreKeyboardInput;

public:
  explicit EnhancedTreeView(QWidget *parent = 0);
  void leaveEvent(QEvent *event);
  void keyPressEvent(QKeyEvent *event);
  bool ignoreKeyboardInput() const { return _ignoreKeyboardInput; }
  void setIgnoreKeyboardInput(bool ignoreKeyboardInput) {
    _ignoreKeyboardInput = ignoreKeyboardInput; }

signals:
  void leaved();
};

#endif // ENHANCEDTREEVIEW_H
