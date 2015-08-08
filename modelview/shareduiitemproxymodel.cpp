#include "shareduiitemproxymodel.h"
#include "modelview/shareduiitem.h"
#include "modelview/shareduiitemsmodel.h"

SharedUiItemProxyModel::SharedUiItemProxyModel(QObject *parent)
  : QIdentityProxyModel(parent) {
}

QVariant SharedUiItemProxyModel::data(
    const QModelIndex &index, int role) const {
  auto suiModel = qobject_cast<SharedUiItemsModel*>(sourceModel());
  if (suiModel && role == Qt::DecorationRole)
    return _icons.value(suiModel->itemAt(mapToSource(index)).idQualifier())
        .value(index.column());
  return QIdentityProxyModel::data(index, role);
}

