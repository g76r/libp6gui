#ifndef TARGETKEEPINGMENU_H
#define TARGETKEEPINGMENU_H

#include <QMenu>
#include "perspectivewidget.h"

/** Menu that keeps previous target when shown.
 * Useful as context menu.
 * @see TargetManager
 */
class LIBH6NCSUSHARED_EXPORT TargetKeepingMenu : public QMenu {
  Q_OBJECT
  Q_DISABLE_COPY(TargetKeepingMenu)
  DtpDocumentManager *_documentManager = 0;
  PerspectiveWidget *_primaryWidget = 0;
  QStringList _primaryItems;

public:
  TargetKeepingMenu(PerspectiveWidget *parent = 0);
  void popup(const QPoint &pos, QAction *atAction = 0);
  void setDocumentManager(DtpDocumentManager *dm) { _documentManager = dm; }
  DtpDocumentManager *documentManger() const { return _documentManager; }

protected:
  void enterEvent(QEvent *event);
};

#endif // TARGETKEEPINGMENU_H
