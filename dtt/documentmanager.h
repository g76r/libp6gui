#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QSet>
#include "targetmanager.h"
#include <QPointer>
#include "tool.h"
#include "libh6ncsu_global.h"

class QKeyEvent;
class QUndoStack;
class PerspectiveWidget;

#define MIMETYPE_TOOL_ID "com.hallowyn/tool-id"
#define MIMETYPE_ITEM_ID "com.hallowyn/item-id"

/** This is a non-visible (i.e. non-widget) class for handling data and events
 * global to a given document.
 * This includes generic features such as: global key handling
 * (i.e. key handling that is neither local to a window nor handled by
 * QShortcut), access to target manager, access to tools...
 * Every application is expected to extends this class and add
 * application-specific features such as: load-save features, data access
 * or version control.
 */
class LIBH6NCSUSHARED_EXPORT DocumentManager : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(DocumentManager)

  class GlobalKey {
    friend class DocumentManager;
    Qt::KeyboardModifiers _modifiers;
    QString _toolId;
  public:
    inline explicit GlobalKey(Qt::KeyboardModifiers modifiers = Qt::NoModifier,
                              QString toolId = QString())
      : _modifiers(modifiers), _toolId(toolId) { }
    inline GlobalKey(const GlobalKey &other) : _modifiers(other._modifiers),
      _toolId(other._toolId) { }
    inline bool isNull() const { return _toolId.isNull(); }
  };

  TargetManager *_targetManager;
  QUndoStack *_undoStack;
  QSet<PerspectiveWidget*> _registredWidgets;
  QHash<QString,QPointer<Tool> > _tools;
  QList<QPointer<Tool> > _permanentTools;
  QPointer<Tool> _currentTool, _tempTool;
  QHash<int,GlobalKey> _globalKeys;

public:
  explicit DocumentManager(QObject *parent);
  ~DocumentManager();
  TargetManager *targetManager() { return _targetManager; }
  virtual void registerWidget(PerspectiveWidget *widget);
  QUndoStack *undoStack() { return _undoStack; }
  /** @return true if the key was intercepted, false if it was ignored and thus
    * should be left to standard Qt processing */
  bool keyPressEvent(QKeyEvent *event);
  const QSet<PerspectiveWidget*> perspectiveWidgets() const {
    return _registredWidgets; }
  QPointer<Tool> currentTool() { return _currentTool; }
  /** return list of permanent (i.e. not drag'n droped or data-related) tools
    * @warning some pointers in the list may be null */
  const QList<QPointer<Tool> > toolsCatalog() { return _permanentTools; }
  /** @warning pointer may be null */
  QPointer<Tool> toolById(const QString id);
  void setGlobalKey(int key, QString toolId = QString(),
                    Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  virtual void unregisterWidget(PerspectiveWidget *widget);
  void addTool(QPointer<Tool> tool, bool permanent = false);
  void addTool(Tool *tool, bool permanent = false) {
    addTool(QPointer<Tool>(tool), permanent); }

signals:
  void currentToolChanged(QPointer<Tool> tool);

private slots:
  void focusChanged(QWidget *oldWidget, QWidget *newWidget);

private:
  void setTempTool(QPointer<Tool> tool) { _tempTool = tool; }
};

#endif // DOCUMENTMANAGER_H
