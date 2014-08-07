#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>
#include <QPointer>
#include "tool.h"
#include "libh6ncsu_global.h"
#include "targetmanager.h"

class DocumentManager;

class LIBH6NCSUSHARED_EXPORT ToolButton : public QAbstractButton {
  Q_OBJECT
  Q_PROPERTY(QColor flashBackground
             READ flashBackground
             WRITE setFlashBackground)
private:
  QPointer<DocumentManager> _documentManager;
  QPointer<Tool> _tool;
  bool _mouseCurrentlyOver, _currentlyTriggerable;
  QString _keyLabel;
  QColor _flashBackground;
  QPoint _mousePressPoint;
  TargetManager::TargetType _targetType;
  int _key;
  Qt::KeyboardModifiers _modifiers;

public:
  explicit ToolButton(QWidget *parent = 0, DocumentManager *documentManager = 0);
  ~ToolButton();
  void setTool(QPointer<Tool> tool);
  void clearTool();
  QString toolTip() const;
  /** @key as in Qt::Key, if 0 remove global key
    */
  void setGlobalKey(int key = 0,
                    Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void enterEvent(QEvent*);
  void leaveEvent(QEvent*);
  void mouseMoveEvent(QMouseEvent *e);
  void dragEnterEvent(QDragEnterEvent *e);
  void dragMoveEvent(QDragMoveEvent *e);
  void dropEvent(QDropEvent *e);
  QColor flashBackground() const { return _flashBackground; }
  void setFlashBackground(QColor color) { _flashBackground = color; update(); }
  void trigger();
  TargetManager::TargetType targetType() const { return _targetType; }
  void setTargetType(TargetManager::TargetType targetType) {
    _targetType = targetType; }
  void setDocumentManager(DocumentManager *documentManager);
  QSize sizeHint() const;

private slots:
  void targetChanged(TargetManager::TargetType targetType,
                     PerspectiveWidget *perspectiveWidget,
                     QStringList itemIds);
  void toolChanged();
  void toolTriggered();
};

#endif // TOOLBUTTON_H
