#ifndef DTPDOCUMENTMANAGERWRAPPER_H
#define DTPDOCUMENTMANAGERWRAPPER_H

#include "dtpdocumentmanager.h"

/** Wrap any SharedUiItemDocumentManager to give it the properties of a
 * DtpDocumentManager. */
class LIBH6NCSUSHARED_EXPORT DtpDocumentManagerWrapper
    : public DtpDocumentManager {
  Q_OBJECT
  Q_DISABLE_COPY(DtpDocumentManagerWrapper)
  SharedUiItemDocumentManager *_wrapped;

public:
  /** Do not take ownership of wrapped document manager, beware that wrapped
   * object must live longer that wrapper. */
  explicit DtpDocumentManagerWrapper(
      SharedUiItemDocumentManager *wrapped, QObject *parent = 0);
  SharedUiItem itemById(QString idQualifier, QString id) const;
  SharedUiItem itemById(QString qualifiedId) const;
  SharedUiItemList<SharedUiItem> itemsByIdQualifier(QString idQualifier) const;
  SharedUiItem createNewItem(QString idQualifier);
  bool changeItemByUiData(SharedUiItem oldItem, int section,
                          const QVariant &value);
  bool changeItem(SharedUiItem newItem, SharedUiItem oldItem);
  void reorderedItems(QList<SharedUiItem> items);
};

#endif // DTPDOCUMENTMANAGERWRAPPER_H
