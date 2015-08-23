#ifndef DTPTREEVIEW_H
#define DTPTREEVIEW_H

#include "enhancedtreeview.h"
#include "dtp/perspectivewidget.h"
#include "modelview/shareduiitem.h"
#include <QPersistentModelIndex>
#include "modelview/shareduiitemsmodel.h"

class TargetManager;

/** Document-Target-Perspective enabled TreeView, with features such as target
 * management according to widget focus, selection and mouse over. */
class LIBH6NCSUSHARED_EXPORT DtpTreeView : public EnhancedTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(DtpTreeView)
  Q_PROPERTY(QStringList primaryItemsIds READ selectedItemsIds
             NOTIFY selectedItemsChanged)
  Q_PROPERTY(QStringList mouseoverItemsIds READ mouseoverItemsIds)
  Q_PROPERTY(PerspectiveWidget* perspectiveWidget READ perspectiveWidget)
  PerspectiveWidget *_perspectiveWidget;
  QPersistentModelIndex _mousePosition;
  QStringList _selectedItemsIds;
  SharedUiItemsProxyModelHelper _proxyModelHelper;

public:
  explicit DtpTreeView(QWidget *parent = 0);
  void setPerspectiveWidget(PerspectiveWidget *widget);
  PerspectiveWidget *perspectiveWidget() const { return _perspectiveWidget; }
  void setModel(QAbstractItemModel *newModel);
  QStringList selectedItemsIds() const { return _selectedItemsIds; }
  //QPersistentModelIndex mousePosition() const { return _mousePosition; }
  bool startItemEdition(QString qualifiedId);

signals:
  void selectedItemsChanged(QStringList selectedItemsIds);

protected:
  void selectionChanged(const QItemSelection &selected,
                        const QItemSelection &deselected) override;
  void commitData(QWidget *editor);

private slots:
  void itemChanged(SharedUiItem newItem, SharedUiItem oldItem);

private:
  void itemHovered(const QModelIndex &index);
  void setMouseoverTarget(QString itemId);
  void setEmptyMouseoverTarget() { setMouseoverTarget(QString()); }
  void clearMouseoverTarget();
  QStringList mouseoverItemsIds() const;
};

#endif // DTPTREEVIEW_H
