#ifndef COLLAPSIBLEGROUPBOX_H
#define COLLAPSIBLEGROUPBOX_H

#include <QGroupBox>
#include "libp6gui_global.h"

class LIBP6GUISHARED_EXPORT CollapsibleGroupBox : public QGroupBox {
  Q_OBJECT
  Q_DISABLE_COPY(CollapsibleGroupBox)

public:
  explicit CollapsibleGroupBox(QWidget *parent = 0);

signals:
  void collapsed();
  void expanded();

public slots:
  void collapse(bool collapsing = true) { doCollapse(collapsing); }
  void expand(bool expanding = true) { doCollapse(!expanding); }

private:
  void doCollapse(bool collapsing);
};

#endif // COLLAPSIBLEGROUPBOX_H
