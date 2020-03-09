//
// Created by christopher on 09.03.2020.
//

#include "ConnectionPage.h"

#include <QAbstractButton>

namespace e4streamer::view {

ConnectionPage::ConnectionPage(QWidget *parent) : QWizardPage(parent), is_connected_(false) {
  this->setCommitPage(true);
}

void e4streamer::view::ConnectionPage::initializePage() {
  QWizardPage::initializePage();

  QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
  button->setText(tr("Connect"));
  button->setEnabled(false);
  button->setVisible(true);
  QObject::connect(button, &QAbstractButton::clicked, [&](bool) {
	QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
	button->setEnabled(false);
	button->setText(tr("Connecting..."));
	this->connect();
  });
}

bool ConnectionPage::validatePage() {
  this->wizard()->button(QWizard::CustomButton1)->setVisible(false);
  return QWizardPage::validatePage();
}

void ConnectionPage::setConnected(bool is_connected) {
  if (is_connected == is_connected_) {
	return;
  }

  is_connected_ = is_connected;
  if (!is_connected) {
	QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
	button->setEnabled(true);
	button->setText(tr("Connect"));
  } else {
	this->wizard()->button(QWizard::CustomButton1)->setText("Connected!");
	emit this->completeChanged();
  }
}

bool ConnectionPage::isComplete() const {
  return QWizardPage::isComplete() && is_connected_;
}

}