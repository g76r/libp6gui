/* Copyright 2014-2023 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QPointer>
#include "libp6gui_global.h"
#include "perspectivewidget.h"
#include "modelview/shareduiitem.h"
#include "util/utf8string.h"

/** Class for accessing and modifying GUI targets.
  * For every target (primary, mouseover...), items ar item ids of targeted
  * DesignItems, widget is the PerspectiveWidget through which the target is
  * acquired (it can be null whereas items list is not empty, if items are
  * targeted outside any perspective, e.g. through version management).
  */
class LIBP6GUISHARED_EXPORT TargetManager : public QObject {
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
  QMap<TargetType,Utf8StringList> _targetItems;
  // TODO not sure how DocumentVersion should be handled
  // should a target contain the DV id and all its items forced to belong to
  // the same DV ?
  // should an itemid be unique within a whole document and therefore DV may be
  // totally ignored by targets semantics ?

public:
  explicit TargetManager(QObject *parent = 0);
  PerspectiveWidget *targetWidget(TargetType targetType = PrimaryTarget) const {
    return _targetWidgets[targetType].data(); }
  Utf8StringList targetItems(TargetType targetType = PrimaryTarget) const {
    return _targetItems[targetType]; }
  /** Must be called each time perspectiveWidget changes or switch to a new
    * perspective and each time targeted items change. */
  void setTarget(TargetType targetType,
                 PerspectiveWidget *perspectiveWidget = 0,
                 Utf8StringList itemsIds = {});
  /** Syntaxic sugar. */
  void setTarget(PerspectiveWidget *perspectiveWidget = 0,
                 Utf8StringList itemsIds = {}) {
    setTarget(PrimaryTarget, perspectiveWidget, itemsIds); }
  /** Syntaxic sugar. */
  void setTarget(TargetType targetType,
                 PerspectiveWidget *perspectiveWidget,
                 QByteArray itemId) {
    setTarget(targetType, perspectiveWidget,
              itemId.isNull() ? QByteArray{} : QByteArray{itemId}); }
  /** Syntaxic sugar. */
  void setTarget(PerspectiveWidget* perspectiveWidget, QByteArray itemId) {
    setTarget(PrimaryTarget, perspectiveWidget,
              itemId.isNull() ? Utf8StringList{} : Utf8StringList{itemId}); }
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
                     Utf8StringList itemIds);
};

#endif // TARGETMANAGER_H
