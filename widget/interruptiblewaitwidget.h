#ifndef INTERRUPTIBLEWAITWIDGET_H
#define INTERRUPTIBLEWAITWIDGET_H

#include <QWidget>
#include "libh6ncsu_global.h"

class QPropertyAnimation;

namespace Ui {
class InterruptibleWaitWidget;
}

/** Widget embedable in forms to control background interruptible tasks such
 * as network requests or external process execution.
 *
 * Display a message along with an endless animated progress bar and an
 * interrupt button. Animation starts on widget show and stops when it's hidden.
 * Signal interruptRequested() is emitted when interrupt button is clicked.
 */
class LIBH6NCSUSHARED_EXPORT InterruptibleWaitWidget : public QWidget {
  Q_OBJECT
  Ui::InterruptibleWaitWidget *ui;
  QPropertyAnimation *_animation;

public:
  explicit InterruptibleWaitWidget(QWidget *parent = 0);
  ~InterruptibleWaitWidget();
  void setMessage(QString text);

protected:
  void showEvent(QShowEvent *event);
  void hideEvent(QHideEvent *event);

signals:
  void interruptRequested();
};

#endif // INTERRUPTIBLEWAITWIDGET_H
