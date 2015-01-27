#include "enhancedplaintextedit.h"
#include <QKeyEvent>

void EnhancedPlainTextEdit::keyPressEvent(QKeyEvent *event) {
  QPlainTextEdit::keyPressEvent(event);
  switch (event->key()) {
  case Qt::Key_Enter:
  case Qt::Key_Return:
      emit editingFinished();
  }
}

void EnhancedPlainTextEdit::focusOutEvent(QFocusEvent *event) {
  QPlainTextEdit::focusOutEvent(event);
  emit editingFinished();
}

