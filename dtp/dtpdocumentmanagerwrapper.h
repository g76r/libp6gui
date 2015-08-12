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

  class ChangeItemCommand : public QUndoCommand {
    QPointer<DtpDocumentManagerWrapper> _dm;
    SharedUiItem _newItem, _oldItem;
    QString _idQualifier;
    bool _firstRedo = true;

  public:
    ChangeItemCommand(DtpDocumentManagerWrapper *dm, SharedUiItem newItem,
                      SharedUiItem oldItem, QString idQualifier,
                      QUndoCommand *parent = 0);
    void redo();
    void undo();
  };
  friend class ChangeItemCommand;

public:
  /** Do not take ownership of wrapped document manager, beware that wrapped
   * object must live longer that wrapper. */
  explicit DtpDocumentManagerWrapper(
      SharedUiItemDocumentManager *wrapped, QObject *parent = 0);
  SharedUiItem itemById(QString idQualifier, QString id) const override;
  SharedUiItem itemById(QString qualifiedId) const override;
  SharedUiItemList<SharedUiItem> itemsByIdQualifier(
      QString idQualifier) const override;
  SharedUiItem createNewItem(
      QString idQualifier, QString *errorString = 0) override;
  bool changeItemByUiData(
      SharedUiItem oldItem, int section, const QVariant &value,
      QString *errorString = 0) override;
  bool changeItem(SharedUiItem newItem, SharedUiItem oldItem,
                  QString idQualifier, QString *errorString = 0) override;
  void reorderedItems(QList<SharedUiItem> items) override;

private:
  void doChangeItem(SharedUiItem newItem, SharedUiItem oldItem,
                    QString idQualifier);
};

#endif // DTPDOCUMENTMANAGERWRAPPER_H
