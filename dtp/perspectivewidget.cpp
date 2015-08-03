#include "perspectivewidget.h"
#include "dtpdocumentmanager.h"
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QtDebug>
#include <QMetaObject>

PerspectiveWidget::PerspectiveWidget(QWidget *parent)
  : QWidget(parent), _documentManager(0) {
}

void PerspectiveWidget::setDocumentManager(DtpDocumentManager *documentManager) {
  _documentManager = documentManager;
  if (!documentManager)
    qWarning() << "PerspectiveWidget::setDocumentManager(0)";
}

DtpDocumentManager *PerspectiveWidget::documentManager() const {
  return _documentManager;
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
      pw->setWindowIcon(window()->windowIcon());
      pw->show();
      return pw;
    }
  }
  return 0;
}

TargetManager *PerspectiveWidget::targetManager(
    PerspectiveWidget *perspectiveWidget) {
  if (perspectiveWidget) {
    DtpDocumentManager *dm = perspectiveWidget->documentManager();
    if (dm)
      return dm->targetManager();
  }
  return 0;
}
