#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QPointer>
#include "libh6ncsu_global.h"
#include "perspectivewidget.h"
#include "modelview/shareduiitem.h"

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
    // MAYDO add a TemplateTarget or AuxiliaryTarget or DragTarget to handle
    // a-la-Visio drag'n drop action through Tools that use such a target
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
                 QStringList itemsIds = QStringList());
  /** Syntaxic sugar. */
  void setTarget(TargetType targetType, PerspectiveWidget* perspectiveWidget,
                 QStringList itemsIds = QStringList()) {
    setTarget(targetType, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(QPointer<PerspectiveWidget> perspectiveWidget
                 = QPointer<PerspectiveWidget>(),
                 QStringList itemsIds = QStringList()) {
    setTarget(PrimaryTarget, perspectiveWidget, itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(PerspectiveWidget* perspectiveWidget,
                 QStringList itemsIds = QStringList()) {
    setTarget(PrimaryTarget, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(TargetType targetType,
                 QPointer<PerspectiveWidget> perspectiveWidget,
                 QString itemId) {
    setTarget(targetType, perspectiveWidget,
              itemId.isNull() ? QStringList() : QStringList(itemId)); }
  /** Syntaxic sugar. */
  void setTarget(TargetType targetType, PerspectiveWidget* perspectiveWidget,
                 QString itemId) {
    setTarget(targetType, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemId.isNull() ? QStringList() : QStringList(itemId)); }
  /** Syntaxic sugar. */
  void setTarget(QPointer<PerspectiveWidget> perspectiveWidget,
                 QString itemId) {
    setTarget(PrimaryTarget, perspectiveWidget,
              itemId.isNull() ? QStringList() : QStringList(itemId)); }
  /** Syntaxic sugar. */
  void setTarget(PerspectiveWidget* perspectiveWidget, QString itemId) {
    setTarget(PrimaryTarget, QPointer<PerspectiveWidget>(perspectiveWidget),
              itemId.isNull() ? QStringList() : QStringList(itemId)); }
  /** All existing target types, as a set. */
  static QSet<TargetManager::TargetType> targetTypes();

public slots:
  /** This slot is only usefull for receiving DocumentManager::itemChanged()
   * signals to update items id in targets when an item id changes.
   * This method should not be called or connected from other classes than
   * DocumentManager. */
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

signals:
  /** Sent every time a target changes */
  void targetChanged(TargetManager::TargetType targetType,
                     PerspectiveWidget *perspectiveWidget,
                     QStringList itemIds);
};

#endif // TARGETMANAGER_H
