#ifndef DTTTREEVIEW_H
#define DTTTREEVIEW_H

#include "enhancedtreeview.h"
#include "dtt/perspectivewidget.h"

// TODO comment
class LIBH6NCSUSHARED_EXPORT DttTreeView : public EnhancedTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(DttTreeView)
  QPointer<DocumentManager> _documentManager;
  QPointer<PerspectiveWidget> _perspectiveWidget;
  QModelIndex _mousePosition;

public:
  explicit DttTreeView(QWidget *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  void setModel(QAbstractItemModel *model);
  void setDocumentManager(DocumentManager *dm);
  DocumentManager *documentManager() const;
  void setPrimaryTargetToSelection();
  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
  //QModelIndex mousePosition() const { return _mousePosition; }
  QString mouseoverItemId() const;

protected:
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected);

private slots:
  void itemHovered(const QModelIndex &index);
  void setMouseoverTarget(QString itemId = QString());
  void clearMouseoverTarget();
};

#endif // DTTTREEVIEW_H
