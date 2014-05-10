#include "perspectivewidget.h"
#include "documentmanager.h"
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QtDebug>
#include <QMetaObject>

PerspectiveWidget::PerspectiveWidget(QWidget *parent)
  : QWidget(parent), _documentManager(0) {
}

PerspectiveWidget::~PerspectiveWidget() {
  if (_documentManager)
    _documentManager->unregisterWidget(this);
}

void PerspectiveWidget::setDocumentManager(DocumentManager *documentManager) {
  if (_documentManager)
    _documentManager->unregisterWidget(this);
  _documentManager = documentManager;
  if (documentManager) {
    _documentManager->registerWidget(this);
    if (_documentManager->mainWindow())
      setWindowIcon(_documentManager->mainWindow()->windowIcon());
  } else
    qWarning() << "PerspectiveWidget::setDocumentManager(0)";
}

QPointer<DocumentManager> PerspectiveWidget::documentManager() const {
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
