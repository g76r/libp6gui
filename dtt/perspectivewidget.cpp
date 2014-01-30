#include "perspectivewidget.h"
#include "documentmanager.h"
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QtDebug>

PerspectiveWidget::PerspectiveWidget(QWidget *parent) : QWidget(parent) {
}

PerspectiveWidget::~PerspectiveWidget() {
  if (_documentManager)
    _documentManager->unregisterWidget(this);
}

void PerspectiveWidget::setDocumentManager(DocumentManager *documentManager) {
  if (_documentManager)
    _documentManager->unregisterWidget(this);
  _documentManager = documentManager;
  if (documentManager)
    _documentManager->registerWidget(this);
  else
    qWarning() << "PerspectiveWidget::setDocumentManager(0)";
}

QPointer<DocumentManager> PerspectiveWidget::documentManager() const {
  return _documentManager;
}

void PerspectiveWidget::keyPressEvent(QKeyEvent *event) {
  if (parent() || !_documentManager
      || !_documentManager->keyPressEvent(event))
    QWidget::keyPressEvent(event);
}

void PerspectiveWidget::startItemEdition(QString itemId) {
  Q_UNUSED(itemId)
  // do nothing
}
