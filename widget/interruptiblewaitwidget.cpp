#include "interruptiblewaitwidget.h"
#include "ui_interruptiblewaitwidget.h"
#include <QPropertyAnimation>

InterruptibleWaitWidget::InterruptibleWaitWidget(QWidget *parent)
  : QWidget(parent), ui(new Ui::InterruptibleWaitWidget) {
  ui->setupUi(this);
  _animation = new QPropertyAnimation(ui->progressBar, "value", this);
  _animation->setDuration(1500);
  _animation->setStartValue(ui->progressBar->minimum());
  _animation->setEndValue(ui->progressBar->maximum());
  //_animation->setEndValue(ui->progressBar->minimum());
  //_animation->setKeyValueAt(.5, ui->progressBar->maximum());
  _animation->setLoopCount(-1);
  connect(ui->interruptButton, &QPushButton::clicked,
          this, &InterruptibleWaitWidget::interruptRequested);
}

InterruptibleWaitWidget::~InterruptibleWaitWidget() {
  delete ui;
}

void InterruptibleWaitWidget::setMessage(QString text) {
  ui->label->setText(text);
}

void InterruptibleWaitWidget::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  _animation->start();
}

void InterruptibleWaitWidget::hideEvent(QHideEvent *event) {
  _animation->stop();
  QWidget::hideEvent(event);
}

