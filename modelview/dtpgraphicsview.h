#ifndef DTPGRAPHICSVIEW_H
#define DTPGRAPHICSVIEW_H

#include "libh6ncsu_global.h"
#include "enhancedgraphicsview.h"
#include "dtp/perspectivewidget.h"
#include <QPointer>

class LIBH6NCSUSHARED_EXPORT DtpGraphicsView : public EnhancedGraphicsView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpGraphicsView)
  QPointer<PerspectiveWidget> _perspectiveWidget;

public:
  explicit DtpGraphicsView(QWidget *parent = 0);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);
  void setScene(QGraphicsScene *scene);

signals:
  /** Same as DtpGraphicsScene::selectedItemsChanged() but only emited when
   * this view has focus */
  void selectedItemsChanged(QStringList selectedItemsIds);
};

#endif // DTPGRAPHICSVIEW_H
