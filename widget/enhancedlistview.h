#ifndef ENHANCEDLISTVIEW_H
#define ENHANCEDLISTVIEW_H

#include <QListView>
#include "libh6ncsu_global.h"

class LIBH6NCSUSHARED_EXPORT EnhancedListView : public QListView {
  Q_OBJECT
  Q_PROPERTY(bool ignoreInternalDrop READ ignoreInternalDrop
             WRITE setIgnoreInternalDrop)
  Q_PROPERTY(bool ignoreKeyboardInput READ ignoreKeyboardInput
             WRITE setIgnoreKeyboardInput)
  bool _ignoreInternalDrop;
  bool _ignoreKeyboardInput;

public:
  explicit EnhancedListView(QWidget *parent = 0);
  void dropEvent(QDropEvent *e);
  void keyPressEvent(QKeyEvent *event);
  inline bool ignoreInternalDrop() const { return _ignoreInternalDrop; }
  inline void setIgnoreInternalDrop(bool ignoreInternalDrop) {
    _ignoreInternalDrop = ignoreInternalDrop; }
  bool ignoreKeyboardInput() const { return _ignoreKeyboardInput; }
  void setIgnoreKeyboardInput(bool ignoreKeyboardInput) {
    _ignoreKeyboardInput = ignoreKeyboardInput; }
};

#endif // ENHANCEDLISTVIEW_H
