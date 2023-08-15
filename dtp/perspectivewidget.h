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
#ifndef PERSPECTIVEWIDGET_H
#define PERSPECTIVEWIDGET_H

#include "libp6gui_global.h"
#include <QWidget>

class DtpDocumentManager;
class DocumentVersion;
class Perspective;
class ItemPosition;
class TargetManager;
class QTabWidget;

/** Parent class for widgets/forms offering a GUI on the document through a
  * perspective.
  * Apart from holding the association to a DocumentManager and being resposible
  * for providing the currently displayed perspective id, the class provides
  * interaction behavior such as being clonable as a top level widget (= a
  * standalone window).
  * @see DocumentManager */
class LIBP6GUISHARED_EXPORT PerspectiveWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(QString perspectiveItemQualifiedId READ perspectiveItemQualifiedId
             WRITE setPerspectiveItemQualifiedId
             NOTIFY perspectiveItemQualifiedIdChanged)
private:
  DtpDocumentManager *_documentManager;
  QString _perspectiveItemQualifiedId;

public:
  explicit PerspectiveWidget(QWidget *parent);
  virtual void setDocumentManager(DtpDocumentManager *documentManager);
  DtpDocumentManager *documentManager() const;
  /** Id of currently displayed perspective */
  virtual QString currentPerspectiveId() const = 0;
  /** Convenience method */
  TargetManager *targetManager() {return targetManager(this); }
  /** Convenience method */
  static TargetManager *targetManager(PerspectiveWidget *pw);
  /** Return id of an item that is associated to the perspective, i.e.
   * if the perspective is viewing the document through this item point
   * of view, for instance by editing only this item and its children/
   * relatives. Can be left empty forever by any implementation. */
  QString perspectiveItemQualifiedId() const {
    return _perspectiveItemQualifiedId; }
  void setPerspectiveItemQualifiedId(QString qualifiedId = QString()) {
    if (_perspectiveItemQualifiedId == qualifiedId)
      return;
    _perspectiveItemQualifiedId = qualifiedId;
    emit perspectiveItemQualifiedIdChanged(qualifiedId); }
  /** Mouse position within the model.
   * Can be usefull to decide an item position within the model when
   * creating it.
   * Position can be screen or widget coordinates or any other coordinates
   * system that fit better the model (more or less than 2 dimensions, etc.)
   * hence the QVariant.
   * Default implementation always return QVariant() */
  virtual QVariant mouseOverPosition() const;

public slots:
  /** Create and a new identical widget (clone) showing same perspective and
    * make it visible as a top level widget (pop).
    * If child class has a Q_INVOKABLE default constructor, use it to create
    * a new instance, set the new instance to the same DocumentManager,
    * set Qt::WA_DeleteOnClose attribute, unset Qt::WA_QuitOnClose attribute
    * and show the widget (as a top level window).
    * @return a pointer on new widget
    * @see copyCloneSharedData() */
  // LATER remove virtual and final keywords
  virtual PerspectiveWidget *popClone() final;
  /** If item found: ensure visible and set it current. If item writeable: start
    * editing it.
    * In this widget or, if not found, recursively within its children, or, if
    * still not found, recursively within its ancestors and siblings.
    * This method is called e.g. when an item has just been added interactively.
    * @return true iff item found */
  // LATER remove virtual and final keywords
  virtual bool startItemEdition(QString qualifiedId) final;
  /** Helper for forms and windows that dispatch startItemEditionHere() among
   * forms within a QTabWidget. Try to call recursiveStartItemEdition() for every
   * PerspectiveWidget within the tab, starting with current one, then first one
   * (index 0). If a call is successful, the tab is made current and true is
   * returned.
   * @return true iff item found */
  static bool startItemEditionInTabWidget(
      QString qualifiedId, QTabWidget *tabWidget);

signals:
  void perspectiveItemQualifiedIdChanged(QString perspectiveItemQualifiedId);

protected:
  /** If item found: ensure visible and set it current. If item writeable: start
    * editing it.
    * This method is called through startItemEdition() e.g. when an item has
    * just been added interactively.
    * Implementation must not recursively try to call children or parent widgets
    * since this is already done by startItemEdition(), it must only start
    * edition on one of its item views, e.g. calling startItemEdition on the
    * right DtpTreeView depending on item's id qualifier.
    */
  virtual bool startItemEditionHere(QString qualifiedId);
  /** Copy data shared among cloned perspectives. Called by popClone().
   * Default impl: copy document manager and window icon.
   * Can be extended to copy additionnal data/resources before or after.
   * @param newWidget newly created widget, not yet shown.
   * @see popClone() */
  virtual void copyCloneSharedData(PerspectiveWidget *newWidget) const;
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent *event) override;

private:
  /** Used internally by startItemEdition() */
  inline bool recursiveStartItemEdition(
      QString qualifiedId, PerspectiveWidget *callingChild = 0);
  /** Used by recursiveStartItemEdition to find PerspectiveWidget within
   * non-PerspectiveWidget descendants. */
  static inline bool recursiveStartItemEditionThroughNonPW(
      QString qualifiedId, QWidget *widget);
  void screenChanged(QScreen *screen);
};

#endif // PERSPECTIVEWIDGET_H
