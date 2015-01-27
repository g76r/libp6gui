#ifndef ENHANCEDPLAINTEXTEDIT_H
#define ENHANCEDPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include "libh6ncsu_global.h"

/** Enhanced QPlainTextEdit
 *
 * Additionnal signals:
 * - editingFinished(): emited whenever enter/return is pressed or focus is lost
 *   (like QAbstractSpinBox::editingFinished()).
 */
class LIBH6NCSUSHARED_EXPORT EnhancedPlainTextEdit : public QPlainTextEdit {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedPlainTextEdit)

public:
  explicit EnhancedPlainTextEdit(QWidget *parent = 0)
    : QPlainTextEdit(parent) { }

signals:
  void editingFinished();

protected:
  void keyPressEvent(QKeyEvent *event);
  void focusOutEvent(QFocusEvent *event);
};

#endif // ENHANCEDPLAINTEXTEDIT_H
