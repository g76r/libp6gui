#include "dtpaction.h"
#include "dtpdocumentmanager.h"
#include "perspectivewidget.h"

DtpAction::DtpAction(DtpDocumentManager *documentManager, QObject *parent)
  : QAction(parent), _documentManager(documentManager) {
}
