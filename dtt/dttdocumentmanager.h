#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QSet>
#include "targetmanager.h"
#include <QPointer>
#include "tool.h"
#include "libh6ncsu_global.h"
#include "toolbutton.h"
#include "mainwindow.h"
#include "modelview/shareduiitemdocumentmanager.h"

class QKeyEvent;
class QUndoStack;
class PerspectiveWidget;

// TODO use enums rather than #defines
#define MIMETYPE_TOOL_ID "com.hallowyn/tool-id"
#define MIMETYPE_ITEM_ID "com.hallowyn/item-id"

// FIXME rename file
/** This is a non-visible (i.e. non-widget) class for handling data and events
 * global to a given document.
 * This includes generic features such as: global key handling
 * (i.e. key handling that is neither local to a window nor handled by
 * QShortcut), access to target manager, access to tools...
 * Every application is expected to extends this class and add
 * application-specific features such as: load-save features, data access
 * or version control.
 */
class LIBH6NCSUSHARED_EXPORT DttDocumentManager
    : public SharedUiItemDocumentManager {
  Q_OBJECT
  Q_DISABLE_COPY(DttDocumentManager)

  class GlobalKey {
  public:
    Qt::KeyboardModifiers _modifiers;
    QPointer<Tool> _tool;
    QPointer<ToolButton> _toolButton;
    GlobalKey() { }
    explicit GlobalKey(Qt::KeyboardModifiers modifiers, QPointer<Tool> tool)
      : _modifiers(modifiers), _tool(tool) { }
    explicit GlobalKey(Qt::KeyboardModifiers modifiers,
                       QPointer<ToolButton> toolButton)
      : _modifiers(modifiers), _toolButton(toolButton) { }
    bool isNull() const { return !_tool && !_toolButton; }
  };

  TargetManager *_targetManager;
  QUndoStack *_undoStack;
  QHash<QString,QPointer<Tool> > _tools;
  QList<QPointer<Tool> > _permanentTools;
  QPointer<Tool> _currentTool, _tempTool;
  QHash<int,GlobalKey> _globalKeys;
  QPointer<MainWindow> _mainWindow;

public:
  explicit DttDocumentManager(QObject *parent = 0);
  TargetManager *targetManager() { return _targetManager; }
  QUndoStack *undoStack() { return _undoStack; }
  /** @return true if the key was intercepted, false if it was ignored and thus
    * should be left to standard Qt processing */
  bool keyPressEvent(QKeyEvent *event);
  QPointer<Tool> currentTool() { return _currentTool; }
  /** return list of permanent (i.e. not drag'n droped or data-related) tools
    * @warning some pointers in the list may be null */
  const QList<QPointer<Tool> > toolsCatalog() { return _permanentTools; }
  /** @warning pointer may be null */
  QPointer<Tool> toolById(const QString id);
  void setGlobalKey(int key, QString toolId,
                    Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  void setGlobalKey(int key, ToolButton *toolButton,
                    Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  void clearGlobalKey(int key);
  void addTool(QPointer<Tool> tool, bool permanent = false);
  void addTool(Tool *tool, bool permanent = false) {
    addTool(QPointer<Tool>(tool), permanent); }
  MainWindow *mainWindow() const;
  void setMainWindow(MainWindow *mainWindow);

signals:
  void currentToolChanged(QPointer<Tool> tool);

private:
  void setTempTool(QPointer<Tool> tool) { _tempTool = tool; }
};

#endif // DOCUMENTMANAGER_H
