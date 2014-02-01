#ifndef TOOL_H
#define TOOL_H

#include <QPointer>
#include <QList>
#include <QString>
#include <QIcon>
#include "libh6ncsu_global.h"
#include "targetmanager.h"
#include <QSet>

class QAction;
class PerspectiveWidget;
class DocumentManager;

/** Tool has nearly the same usage than QAction, however it is target-aware
  * and therefore able to change its behaviour or appearance depending on
  * TargetManager events.
  * It provides a QAction wrapper to be used whereever the Qt standard classes
  * expect a QAction object, for instance in menus.
  * Most action in the software should be Tools rather than QActions, however
  * there may be some natural QActions for actions that has no real interest
  * neither in being target-aware nor in being drag'n dropable inn tool widgets.
  */
class LIBH6NCSUSHARED_EXPORT Tool : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Tool)

  QPointer<DocumentManager> _documentManager;
  QString _id;
  QString _label;
  QIcon _icon;
  bool _enabled;
  QAction *_action;
  QSet<TargetManager::TargetType> _acceptedTargets;
  TargetManager::TargetType _preferredTarget;

public:
  /** @param acceptedTargets if empty, replaced with all targets
   * @param preferredTarget if not in accepteTargets, replaced with first
   * accepted target
   * */
  Tool(DocumentManager *parent, const QString id,
       QSet<TargetManager::TargetType> acceptedTargets
       = QSet<TargetManager::TargetType>(),
       TargetManager::TargetType preferredTarget
       = TargetManager::PrimaryTarget);
  ~Tool();
  QPointer<DocumentManager> documentManager() const;
  const QString id() const { return _id; }
  const QString label() const { return _label; }
  void setLabel(QString label);
  const QIcon icon() const { return _icon; }
  void setIcon(QIcon icon);
  bool enabled() const { return _enabled; }
  void setEnabled(bool enabled);
  /** return true if enabled and e.g. target is valid
    * default: return enabled() */
  virtual bool triggerable(TargetManager::TargetType targetType) const;
  /** Provide access to the QAction wrapper. Tool keeps QAction ownership */
  inline QAction *action() const { return _action; }
  // LATER add possibility to create widget on trigger, see QWidgetAction
  QSet<TargetManager::TargetType> acceptedTargets() const {
    return _acceptedTargets; }
  TargetManager::TargetType preferredTarget() const {
    return _preferredTarget; }
  /** call trigger(preferredTarget()) */
  virtual void trigger(TargetManager::TargetType targetType);

public slots:
  void trigger();

signals:
  /** Notify associated widgets that something in the tool appearance has
    * changed. E.g. it has been enabled or disabled, its label changed, etc. */
  void changed();
  void triggered();
};

#endif // TOOL_H
