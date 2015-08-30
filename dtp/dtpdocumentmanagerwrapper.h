#ifndef DTPDOCUMENTMANAGERWRAPPER_H
#define DTPDOCUMENTMANAGERWRAPPER_H

#include "dtpdocumentmanager.h"
#include <QUndoCommand>

/** Wrap any SharedUiItemDocumentManager to give it the properties of a
 * DtpDocumentManager.
 *
 * If wrapped DM inherits from InMemorySharedUiItemDocumentManager, it will
 * have all its change operations recorded in the undo stack.
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
  SharedUiItem createNewItem(QString idQualifier, QString *errorString = 0);
  bool changeItem(SharedUiItem newItem, SharedUiItem oldItem,
                  QString idQualifier, QString *errorString = 0);
  bool changeItemByUiData(SharedUiItem oldItem, int section,
                          const QVariant &value, QString *errorString = 0);

protected:
  bool prepareChangeItem(
      QUndoCommand *command, SharedUiItem newItem, SharedUiItem oldItem,
      QString idQualifier, QString *errorString) override;
  void commitChangeItem(SharedUiItem newItem, SharedUiItem oldItem,
                        QString idQualifier) override;
};

#endif // DTPDOCUMENTMANAGERWRAPPER_H
