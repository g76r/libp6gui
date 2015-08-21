#include "perspectivewidget.h"
#include "dtpdocumentmanager.h"
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QtDebug>
#include <QMetaObject>
#include "widget/widgetutils.h"

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

bool PerspectiveWidget::startItemEdition(QString qualifiedId) {
  return recursiveStartItemEdition(qualifiedId)
      || recursiveStartItemEdition(qualifiedId, this);
}

bool PerspectiveWidget::recursiveStartItemEditionThroughNonPW(
    QString qualifiedId, QWidget *widget) {
  auto *pw = qobject_cast<PerspectiveWidget*>(widget);
  if (pw)
    return pw->recursiveStartItemEdition(qualifiedId);
  foreach (QObject *child, widget->children()) {
    auto *widget = qobject_cast<QWidget*>(child);
    if (widget && recursiveStartItemEditionThroughNonPW(qualifiedId, widget))
      return true;
  }
  return false;
}

bool PerspectiveWidget::recursiveStartItemEdition(
    QString qualifiedId, PerspectiveWidget *callingChild) {
  // first: try to start item edition in this widget
  if (startItemEditionHere(qualifiedId)) {
    WidgetUtils::ensureWidgetIsVisible(this);
    return true;
  }
  // otherwise: try in children, excepted in recursively calling child
  foreach (QObject *child, children()) {
    auto *widget = qobject_cast<QWidget*>(child);
    if (widget && widget != callingChild
        && recursiveStartItemEditionThroughNonPW(qualifiedId, widget)) {
      return true;
    }
  }
  // otherwise, if upward recursion is enabled, try in parent
  if (callingChild) {
  auto *pw = qobject_cast<PerspectiveWidget*>(parent());
  if (pw)
    return pw->recursiveStartItemEdition(qualifiedId, this);
  }
  // otherwise the item was not found
  return false;
}

bool PerspectiveWidget::startItemEditionHere(QString qualifiedId) {
  Q_UNUSED(qualifiedId)
  return false;
}

PerspectiveWidget *PerspectiveWidget::popClone() {
  if (_documentManager) {
    PerspectiveWidget *pw =
        qobject_cast<PerspectiveWidget*>(metaObject()->newInstance());
    if (pw) {
      pw->setAttribute(Qt::WA_QuitOnClose, false);
      pw->setAttribute(Qt::WA_DeleteOnClose);
      copyCloneSharedData(pw);
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

void PerspectiveWidget::copyCloneSharedData(
    PerspectiveWidget *newWidget) const {
  newWidget->setDocumentManager(documentManager());
  newWidget->setWindowIcon(window()->windowIcon());
}
