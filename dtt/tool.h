#ifndef TOOL_H
#define TOOL_H

#include <QPointer>
#include <QList>
#include <QString>
#include <QIcon>
#include "libh6ncsu_global.h"
#include "targetmanager.h"

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
private:
  QPointer<DocumentManager> _documentManager;
  QString _id;
  QString _label;
  QIcon _icon;
  bool _enabled;
  QAction *_action;

public:
  Tool(DocumentManager *parent, const QString id);
  ~Tool();
  QPointer<DocumentManager> documentManager() const;
  const QString id() const { return _id; }
  const QString label() const { return _label; }
  void setLabel(QString label);
  const QIcon icon() const { return _icon; }
  void setIcon(QIcon icon);
  bool enabled() const { return _enabled; }
  void setEnabled(bool enabled);
  /** Provide access to the QAction wrapper. Tool keeps QAction ownership */
  inline QAction *action() const { return _action; }
  // LATER add possibility to create widget on trigger, see QWidgetAction

public slots:
  // FIXME add target type
  virtual void trigger();

private slots:
  // LATER rather ToolButton should be warned of target changes ? or connect only tools that are bound to a widget ?
  virtual void targetChanged(TargetManager::TargetType targetType,
                             PerspectiveWidget *perspectiveWidget,
                             QStringList itemIds);

signals:
  /** Notify associated widgets that something in the tool appearance has
    * changed. E.g. it has been enabled or disabled, its label changed, etc. */
  void changed();
  void triggered();
};

#endif // TOOL_H
