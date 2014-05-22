#ifndef SHAREDUIGRAPHICSITEM_H
#define SHAREDUIGRAPHICSITEM_H

#include "libh6ncsu_global.h"
#include <QGraphicsObject>
#include "modelview/shareduiitem.h"

/** QGraphicsObject holding a SharedUiItem.
 * Should be specialized depending on needs, at less for painting purposes. */
class LIBH6NCSUSHARED_EXPORT SharedUiGraphicsItem : public QGraphicsObject {
  Q_OBJECT
  Q_DISABLE_COPY(SharedUiGraphicsItem)
  // LATER declare id and qualifiedId as Q_PROPERTYs
  SharedUiItem _uiItem;

public:
  SharedUiGraphicsItem(QGraphicsItem *parent = 0);
  SharedUiGraphicsItem(SharedUiItem uiItem, QGraphicsItem *parent = 0);
  SharedUiItem uiItem() const { return _uiItem; }
  void setUiItem(SharedUiItem uiItem) { _uiItem = uiItem; }
  /** Syntaxic sugar - proxy on SharedUiItem */
  bool isNull() const { return _uiItem.isNull(); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QVariant uiHeaderData(int section, int role) const {
    return _uiItem.uiHeaderData(section, role); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QString uiHeaderString(int section, int role = Qt::DisplayRole) const {
    return _uiItem.uiHeaderString(section, role); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  int uiDataCount() const { return _uiItem.uiDataCount(); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QVariant uiData(int section, int role = Qt::DisplayRole) const {
    return _uiItem.uiData(section, role); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QString uiString(int section, int role = Qt::DisplayRole) const {
    return _uiItem.uiString(section, role); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QString id() const { return _uiItem.id(); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QString idQualifier() const { return _uiItem.idQualifier(); }
  /** Syntaxic sugar - proxy on SharedUiItem */
  QString qualifiedId() const { return _uiItem.qualifiedId(); }
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // SHAREDUIGRAPHICSITEM_H
