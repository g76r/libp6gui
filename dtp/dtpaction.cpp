#include "dtpaction.h"
#include "dtpdocumentmanager.h"
#include "perspectivewidget.h"

DtpAction::DtpAction(DtpDocumentManager *documentManager, const QString id)
  : QAction(documentManager), _documentManager(documentManager), _id(id) {
  if (documentManager)
    documentManager->addAction(this);
}
