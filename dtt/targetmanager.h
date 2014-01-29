#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QPointer>
#include "libh6ncsu_global.h"
#include "perspectivewidget.h"

/** Class for accessing and modifying GUI targets.
  * For every target (primary, mouseover...), items ar item ids of targeted
  * DesignItems, widget is the PerspectiveWidget through which the target is
  * acquired (it can be null whereas items list is not empty, if items are
  * targeted outside any perspective, e.g. through version management).
  */
class LIBH6NCSUSHARED_EXPORT TargetManager : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(TargetManager)

public:
  enum TargetType {
    PrimaryTarget,
    PreviousPrimaryTarget,
    MouseOverTarget
  };

private:
  QMap<TargetType,QPointer<PerspectiveWidget> > _targetWidgets;
  QMap<TargetType,QStringList> _targetItems;
  // TODO not sure how DocumentVersion should be handled
  // should a target contain the DV id and all its items forced to belong to
  // the same DV ?
  // should an itemid be unique within a whole document and therefore DV may be
  // totally ignored by targets semantics ?

public:
  explicit TargetManager(QObject *parent = 0);
  PerspectiveWidget *targetWidget(TargetType targetType = PrimaryTarget) const {
    return _targetWidgets[targetType].data(); }
  QStringList targetItems(TargetType targetType = PrimaryTarget) const {
    return _targetItems[targetType]; }
  /** Must be called each time perspectiveWidget changes or switch to a new
    * perspective and each time targeted items change. */
  void setTarget(TargetType targetType,
                 QPointer<PerspectiveWidget> perspectiveWidget
                 = QPointer<PerspectiveWidget>(),
                 QList<QString> itemsIds = QList<QString>());
  /** Syntaxic sugar. */
  void setTarget(TargetType targetType, PerspectiveWidget* perspectiveWidget,
                 QList<QString> itemsIds = QList<QString>()) {
    setTarget(targetType, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(QPointer<PerspectiveWidget> perspectiveWidget
                 = QPointer<PerspectiveWidget>(),
                 QList<QString> itemsIds = QList<QString>()) {
    setTarget(PrimaryTarget, perspectiveWidget, itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(PerspectiveWidget* perspectiveWidget,
                 QList<QString> itemsIds = QList<QString>()) {
    setTarget(PrimaryTarget, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemsIds); }

signals:
  /** Sent every time a target changes */
  void targetChanged(TargetType targetType,
                     PerspectiveWidget *perspectiveWidget,
                     QList<QString> itemIds);
};

#endif // TARGETMANAGER_H
