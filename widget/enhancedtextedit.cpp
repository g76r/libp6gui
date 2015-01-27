#include "enhancedtextedit.h"
#include <QKeyEvent>

void EnhancedTextEdit::keyPressEvent(QKeyEvent *event) {
  QTextEdit::keyPressEvent(event);
  switch (event->key()) {
  case Qt::Key_Enter:
  case Qt::Key_Return:
      emit editingFinished();
  }
}

void EnhancedTextEdit::focusOutEvent(QFocusEvent *event) {
  QTextEdit::focusOutEvent(event);
  emit editingFinished();
}

