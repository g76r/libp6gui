#ifndef PERSPECTIVESTACKEDWIDGET_H
#define PERSPECTIVESTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPointer>
#include "perspectivewidget.h"
#include "libh6ncsu_global.h"

class DttDocumentManager;

/** This class is an helper for view design form which stacks several widgets
  * page and wants mouseover target handling being consistant over the stacked
  * widget. Nothing more. */
class LIBH6NCSUSHARED_EXPORT PerspectiveStackedWidget : public QStackedWidget {
  Q_OBJECT
  QPointer<PerspectiveWidget> _perspectiveWidget;

public:
  explicit PerspectiveStackedWidget(PerspectiveWidget *parent);
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);

public slots:
  void perspectiveChanged();

private:
  inline void setMouseoverTarget(bool set);
};

#endif // PERSPECTIVESTACKEDWIDGET_H
