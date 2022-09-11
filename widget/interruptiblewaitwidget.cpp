/* Copyright 2014-2022 Hallowyn and others.
 * This file is part of libpumpkin, see <http://libpumpkin.g76r.eu/>.
 * libpumpkin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * libpumpkin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with libpumpkin.  If not, see <http://www.gnu.org/licenses/>.
 */
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

