#include "perspectivewidget.h"
#include "dttdocumentmanager.h"
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QtDebug>
#include <QMetaObject>

PerspectiveWidget::PerspectiveWidget(QWidget *parent)
  : QWidget(parent), _documentManager(0) {
}

void PerspectiveWidget::setDocumentManager(DttDocumentManager *documentManager) {
  _documentManager = documentManager;
  if (documentManager) {
    if (_documentManager->mainWindow())
      setWindowIcon(_documentManager->mainWindow()->windowIcon());
  } else
    qWarning() << "PerspectiveWidget::setDocumentManager(0)";
}

QPointer<DttDocumentManager> PerspectiveWidget::documentManager() const {
  return _documentManager;
}

void PerspectiveWidget::keyPressEvent(QKeyEvent *event) {
  if (!_documentManager || !_documentManager->keyPressEvent(event))
    QWidget::keyPressEvent(event);
}

void PerspectiveWidget::startItemEdition(QString itemId) {
  Q_UNUSED(itemId)
  // do nothing
}

PerspectiveWidget *PerspectiveWidget::popClone() {
  if (_documentManager) {
    PerspectiveWidget *pw =
        qobject_cast<PerspectiveWidget*>(metaObject()->newInstance());
    if (pw) {
      pw->setAttribute(Qt::WA_QuitOnClose, false);
      pw->setAttribute(Qt::WA_DeleteOnClose);
      pw->setDocumentManager(documentManager());
      pw->show();
      return pw;
    }
  }
  return 0;
}

TargetManager *PerspectiveWidget::targetManager(
    PerspectiveWidget *perspectiveWidget) {
  if (perspectiveWidget) {
    QPointer<DttDocumentManager> dm = perspectiveWidget->documentManager();
    if (dm)
      return dm->targetManager();
  }
  return 0;
}
