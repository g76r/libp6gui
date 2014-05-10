#ifndef DTTLISTVIEW_H
#define DTTLISTVIEW_H

#include "enhancedlistview.h"
#include "dtt/perspectivewidget.h"

class DttListView : public EnhancedListView {
  Q_OBJECT
  Q_DISABLE_COPY(DttListView)
  QPointer<DocumentManager> _documentManager;
  QPointer<PerspectiveWidget> _perspectiveWidget;
  QModelIndex _mousePosition;

public:
  explicit DttListView(QWidget *parent = 0);
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

#endif // DTTLISTVIEW_H
