#ifndef SHAREDUIITEMPROXYMODEL_H
#define SHAREDUIITEMPROXYMODEL_H

#include <QIdentityProxyModel>
#include <QIcon>
#include <QString>
#include <QHash>
#include "libh6ncsu_global.h"

/** Decoration proxy model for SharedUiItemsModel.
 * @see SharedUiItemsModel */
class LIBH6NCSUSHARED_EXPORT SharedUiItemProxyModel
    : public QIdentityProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(SharedUiItemProxyModel)
  QHash<QString,QHash<int,QIcon>> _icons;

public:
  SharedUiItemProxyModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index, int role) const;
  /** Associate an id qualifier with an icon. */
  SharedUiItemProxyModel &setIcon(QString idQualifier, int column, QIcon icon) {
    _icons[idQualifier][column] = icon; return *this; }
  /** Convenience operator() for setIcon(). */
  SharedUiItemProxyModel &operator()(
      QString idQualifier, int column, QIcon icon) {
    return setIcon(idQualifier, column, icon); }
  /** Reset id qualifiers and icons associations. */
  SharedUiItemProxyModel &clearIcons() { _icons.clear(); return *this; }
};

#endif // SHAREDUIITEMPROXYMODEL_H
