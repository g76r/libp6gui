#ifndef DTPDOCUMENTMANAGERWRAPPER_H
#define DTPDOCUMENTMANAGERWRAPPER_H

#include "dtpdocumentmanager.h"
#include <QUndoCommand>

/** Wrap any SharedUiItemDocumentManager to give it the properties of a
 * DtpDocumentManager.
 */
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
  SharedUiItem itemById(QString idQualifier, QString id) const override;
  SharedUiItem itemById(QString qualifiedId) const override;
  SharedUiItemList<SharedUiItem> itemsByIdQualifier(
      QString idQualifier) const override;
  void reorderItems(QList<SharedUiItem> items) override;
  void registerItemType(QString idQualifier, Setter setter,
                        Creator creator) = delete;

protected:
  bool prepareChangeItem(
      CoreUndoCommand *command, SharedUiItem newItem, SharedUiItem oldItem,
      QString idQualifier, QString *errorString) override;
  void commitChangeItem(SharedUiItem newItem, SharedUiItem oldItem,
                        QString idQualifier) override;
  CoreUndoCommand *internalCreateNewItem(
      SharedUiItem *newItem, QString idQualifier,
      QString *errorString) override;
  CoreUndoCommand *internalChangeItem(
      SharedUiItem newItem, SharedUiItem oldItem, QString idQualifier,
      QString *errorString) override;
  CoreUndoCommand *internalChangeItemByUiData(
      SharedUiItem oldItem, int section, const QVariant &value,
      QString *errorString) override;
};

#endif // DTPDOCUMENTMANAGERWRAPPER_H
