#ifndef DTPDOCUMENTMANAGER_H
#define DTPDOCUMENTMANAGER_H

#include <QSet>
#include "targetmanager.h"
#include <QPointer>
#include "dtpaction.h"
#include "libh6ncsu_global.h"
#include "toolbutton.h"
#include "modelview/shareduiitemdocumentmanager.h"

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

public:
  explicit DtpDocumentManager(QObject *parent = 0);
  TargetManager *targetManager() { return _targetManager; }
  QUndoStack *undoStack() { return _undoStack; }
};

#endif // DTPDOCUMENTMANAGER_H
