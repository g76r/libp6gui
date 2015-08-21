#ifndef ENHANCEDTOOLBUTTON_H
#define ENHANCEDTOOLBUTTON_H

#include <QToolButton>
#include "libh6ncsu_global.h"


/** Enhanced QToolButton
 *
 * Changed default behavior:
 * - default focus policy is Qt::NoFocus instead of Qt::TabFocus
 *
 * Additionnal features:
 * - append action shortcut to tooltip by default
 * - animate button on action trigger, even if it has not been triggered through
 *   this button (e.g. if it has been triggered through keyboard shortcut)
 */
class LIBH6NCSUSHARED_EXPORT EnhancedToolButton : public QToolButton {
  Q_OBJECT
  Q_DISABLE_COPY(EnhancedToolButton)

public:
  enum TriggerAnimationMethod { Disabled, AlterStyleSheet, SetDown };

private:
  TriggerAnimationMethod _triggerAnimationMethod = AlterStyleSheet;

public:
  explicit EnhancedToolButton(QWidget *parent = 0);
  void setDefaultAction(QAction *action, bool setRichTooltip = true);
  /** Default: AlterStyleSheet. */
  void setTriggerAnimationMethod(TriggerAnimationMethod method) {
    _triggerAnimationMethod = method; }
  TriggerAnimationMethod triggerAnimationMethod() const {
    return _triggerAnimationMethod; }

private:
  void fakeAnimateClick();

private slots:
  void endOfFakeAnimateClick();
};

#endif // ENHANCEDTOOLBUTTON_H
