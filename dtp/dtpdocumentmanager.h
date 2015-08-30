#ifndef DTPDOCUMENTMANAGER_H
#define DTPDOCUMENTMANAGER_H

#include <QSet>
#include "targetmanager.h"
#include "dtpaction.h"
#include "libh6ncsu_global.h"
#include "toolbutton.h"
#include "modelview/shareduiitemdocumentmanager.h"
#include <QUndoCommand>

class QUndoStack;
class PerspectiveWidget;

// TODO use enums rather than #defines
//#define MIMETYPE_TOOL_ID "com.hallowyn/tool-id"
//#define MIMETYPE_ITEM_ID "com.hallowyn/item-id"

/** DtpDocumentManager is a core class of the Document-Target-Perspective model,
 * this is a non-visible (i.e. non-widget) class for handling data and events
 * global to a given document.
 * This includes generic features such as: access to target manager, access to
 * undo stack...
 * Every application is expected to extends this class and add
 * application-specific features such as: load-save features, data access...
 */
class LIBH6NCSUSHARED_EXPORT DtpDocumentManager
    : public SharedUiItemDocumentManager {
  Q_OBJECT
  Q_DISABLE_COPY(DtpDocumentManager)
  TargetManager *_targetManager;
  QUndoStack *_undoStack;

protected:
  class LIBH6NCSUSHARED_EXPORT ChangeItemCommand : public QUndoCommand {
    QPointer<DtpDocumentManager> _dm;
    SharedUiItem _newItem, _oldItem;
    QString _idQualifier;
    bool _ignoreFirstRedo;

  public:
    ChangeItemCommand(DtpDocumentManager *dm, SharedUiItem newItem,
                      SharedUiItem oldItem, QString idQualifier,
                      QUndoCommand *parent, bool ignoreFirstRedo = false);
    void redo();
    void undo();
  };
  friend class ChangeItemCommand;

public:
  explicit DtpDocumentManager(QObject *parent = 0);
  TargetManager *targetManager() { return _targetManager; }
  QUndoStack *undoStack() { return _undoStack; }
  /** Create a global QUndoCommand and call prepareChangeItem(). */
  bool changeItemByUiData(SharedUiItem oldItem, int section,
                          const QVariant &value,
                          QString *errorString = 0);
  /** Create a global QUndoCommand and call prepareChangeItem(). */
  bool changeItem(SharedUiItem newItem, SharedUiItem oldItem,
                  QString idQualifier, QString *errorString = 0);

protected:
  virtual bool prepareChangeItem(QUndoCommand *command, SharedUiItem newItem,
                                 SharedUiItem oldItem, QString idQualifier,
                                 QString *errorString) = 0;
  virtual void commitChangeItem(SharedUiItem newItem, SharedUiItem oldItem,
                                QString idQualifier) = 0;
};

#endif // DTPDOCUMENTMANAGER_H
