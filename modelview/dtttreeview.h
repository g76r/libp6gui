#ifndef DTTTREEVIEW_H
#define DTTTREEVIEW_H

#include "enhancedtreeview.h"
#include "dtt/perspectivewidget.h"

class TargetManager;

// TODO comment
class LIBH6NCSUSHARED_EXPORT DttTreeView : public EnhancedTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(DttTreeView)
  QPointer<PerspectiveWidget> _perspectiveWidget;
  QModelIndex _mousePosition;
  QModelIndexList _selectedItemsIndexes;

public:
  explicit DttTreeView(QWidget *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  void setModel(QAbstractItemModel *model);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  //QModelIndex mousePosition() const { return _mousePosition; }

signals:
  void selectedItemsChanged(QStringList selectedItemsIds);

protected:
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected);

private slots:
  void itemHovered(const QModelIndex &index);
  void setMouseoverTarget(QString itemId = QString());
  void clearMouseoverTarget();

private:
  QStringList buildSelectedItemsIdsList() const;
  QString mouseoverItemId() const;
};

#endif // DTTTREEVIEW_H
