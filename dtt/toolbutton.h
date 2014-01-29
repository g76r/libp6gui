#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>
#include <QPointer>
#include "tool.h"
#include "libh6ncsu_global.h"

class DocumentManager;

class LIBH6NCSUSHARED_EXPORT ToolButton : public QAbstractButton {
  Q_OBJECT
  Q_PROPERTY(QColor flashBackground
             READ flashBackground
             WRITE setFlashBackground)
private:
  QPointer<DocumentManager> _documentManager;
  QPointer<Tool> _tool;
  bool _mouseover;
  int _key;
  Qt::KeyboardModifiers _modifiers;
  QString _keyLabel;
  QColor _flashBackground;
  QPoint _mousePressPoint;

public:
  explicit ToolButton(DocumentManager *documentManager, QWidget *parent = 0);
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

private slots:
  void toolChanged();
  void toolTriggered();
};

#endif // TOOLBUTTON_H
