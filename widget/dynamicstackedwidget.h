#ifndef DYNAMICSTACKEDWIDGET_H
#define DYNAMICSTACKEDWIDGET_H

#include <QStackedWidget>
#include "libp6gui_global.h"

class LIBP6GUISHARED_EXPORT DynamicStackedWidget : public QStackedWidget {
  Q_OBJECT
  // these properties can be set in QtCreator as a "dynamic property"
  Q_PROPERTY(int expandedIndex READ expandedIndex WRITE setExpandedIndex)
  Q_PROPERTY(int reducedIndex READ reducedIndex WRITE setReducedIndex)
  Q_PROPERTY(bool frozen READ frozen WRITE setFrozen)
  Q_PROPERTY(bool expandsHorizontally READ expandsHorizontally
             WRITE setExpandsHorizontally)
  // this property is only used for the purpose of animation
  Q_PROPERTY(int currentLength READ currentLength WRITE setCurrentLength)

private:
  int _expandedIndex;
  int _reducedIndex;
  bool _frozen;
  bool _shouldBeReduced;
  int _currentLength;
  bool _expandsHorizontally;

public:
  explicit DynamicStackedWidget(QWidget *parent = 0);
  void enterEvent(QEnterEvent *event);
  void leaveEvent(QEvent *event);
  void showEvent(QShowEvent *event);
  int expandedIndex() const { return _expandedIndex; }
  void setExpandedIndex(int expandedIndex) { _expandedIndex = expandedIndex; }
  int reducedIndex() const { return _reducedIndex; }
  void setReducedIndex(int reducedIndex) { _reducedIndex = reducedIndex; }
  bool frozen() const { return _frozen; }
  int currentLength() const { return _currentLength; }
  void setCurrentLength(int currentLength);
  QSize sizeHint() const;
  QSize minimumSizeHint() const;
  bool expandsHorizontally() const { return _expandsHorizontally; }
  void setExpandsHorizontally(bool enabled);

signals:
  void reduced();
  void expanded();

public slots:
  void setFrozen(bool frozen);
  void freeze() { setFrozen(true); }
  void unfreeze() { setFrozen(false); }
  void reduce() { reduce(true); }
  void expand() { reduce(false); }
  // following slots are needed because the order of the two actions is
  // important and defining two connections would not garantee order
  void reduceAndFreeze();
  void reduceAndUnfreeze();
  void expandAndFreeze();

private:
  void reduce(bool reducing, bool shouldAnimate = true);
  Q_DISABLE_COPY(DynamicStackedWidget)
};

#endif // DYNAMICSTACKEDWIDGET_H
