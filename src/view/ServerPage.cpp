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

ServerPage::ServerPage(QWidget *parent) : ConnectionPage(parent), server_config_(new widgets::ServerConfig(this)) {
  this->setTitle(tr("Step 1: Configurate the Empathica Bluetooth server"));
  this->setSubTitle(tr(
	  "Please specify the location and parameters for the Empathica Bluetooth server. It provides the connection to the E4 and enforces the usage of API keys."));

  auto layout = new QVBoxLayout(this);
  layout->addWidget(server_config_);
  this->setLayout(layout);

  // Enable loading when user made usable input
  QObject::connect(server_config_->model(), &model::Server::readinessChanged, [&](bool is_ready) {
	this->connectionButton()->setEnabled(is_ready);
  });

  // Allow continue
  QObject::connect(server_config_->model(), &model::Server::connected, [&](model::Connection *) {
	this->setConnected(true);
  });

  // Allow edit on the view in case of an error
  QObject::connect(server_config_->model(), &model::Server::connectionFailed, [&](const QString &) {
	this->setConnected(false);
	server_config_->setEnabled(true);
  });
}

void ServerPage::initializePage() {
  ConnectionPage::initializePage();
  this->connectionButton()->setEnabled(false);
}

model::Connection *ServerPage::connection() {
  return server_config_->model()->connection();
}

void ServerPage::connect() {
  if (server_config_->model()->start()) {
	server_config_->setEnabled(false);
  }
}
}