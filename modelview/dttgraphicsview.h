#ifndef DTTGRAPHICSVIEW_H
#define DTTGRAPHICSVIEW_H

#include "libh6ncsu_global.h"
#include "enhancedgraphicsview.h"
#include "dtt/perspectivewidget.h"

class LIBH6NCSUSHARED_EXPORT DttGraphicsView : public EnhancedGraphicsView {
  Q_OBJECT
  Q_DISABLE_COPY(DttGraphicsView)
  QPointer<PerspectiveWidget> _perspectiveWidget;

public:
  explicit DttGraphicsView(QWidget *parent = 0);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);
  void setScene(QGraphicsScene *scene);

signals:
  /** same as DttGraphicsScene::selectedItemsChanged() but only emited when
   * this view has focus */
  void selectedItemsChanged(QStringList selectedItemsIds);
};

#endif // DTTGRAPHICSVIEW_H
