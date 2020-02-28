//
// Created by christopher on 27.02.2020.
//

#include "ServerPage.h"
#include "Wizard.h"
#include "widgets/ServerConfig.h"
#include "../model/Server.h"

#include <QAbstractButton>
#include <QVBoxLayout>

namespace e4streamer::view {

ServerPage::ServerPage(QWidget *parent) : QWizardPage(parent), server_config_(new widgets::ServerConfig(this)) {
  this->setSubTitle(tr(
	  "Please specify the location and parameters for the Empathica Bluetooth server. It provides the connection to the E4 and enforces the usage of API keys."));
  this->setCommitPage(true);
  auto layout = new QVBoxLayout(this);
  layout->addWidget(server_config_);
  this->setLayout(layout);

  // Enable loading when user made usable input
  QObject::connect(server_config_->model(), &model::Server::readinessChanged, [&](bool is_ready) {
	this->wizard()->button(QWizard::CustomButton1)->setEnabled(is_ready);
  });

  // Allow edit on the view in case of an error
  QObject::connect(server_config_->model(), &model::Server::connectionFailed, [&](const QString &) {
	QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
	button->setEnabled(true);
	button->setText(tr("Connect"));
	server_config_->setEnabled(true);
  });

  // Allow continue
  QObject::connect(server_config_->model(), &model::Server::connected, [&](model::Connection *) {
	this->wizard()->button(QWizard::CustomButton1)->setText("Connected!");
	emit this->completeChanged();
  });
}

void ServerPage::initializePage() {
  QWizardPage::initializePage();

  QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
  button->setText(tr("Connect"));
  button->setEnabled(false);
  button->setVisible(true);
  QObject::connect(button, &QAbstractButton::clicked, [&](bool) {
	QAbstractButton *button = this->wizard()->button(QWizard::CustomButton1);
	button->setEnabled(false);
	button->setText(tr("Connecting..."));
	if (server_config_->model()->start()) {
	  server_config_->setEnabled(false);
	}
  });
}

bool ServerPage::isComplete() const {
  return QWizardPage::isComplete() && server_config_->model()->state() == model::Server::State::Connected;
}

model::Connection *ServerPage::connection() {
  return server_config_->model()->connection();
}

bool ServerPage::validatePage() {
  this->wizard()->button(QWizard::CustomButton1)->setVisible(false);
  return QWizardPage::validatePage();
}

}