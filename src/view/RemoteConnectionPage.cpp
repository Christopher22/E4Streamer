//
// Created by christopher on 09.03.2020.
//

#include "RemoteConnectionPage.h"
#include "util/PortBox.h"
#include "../model/ConnectionManager.h"
#include "../model/Connection.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QFormLayout>

namespace e4streamer::view {

RemoteConnectionPage::RemoteConnectionPage(QWidget *parent)
	: ConnectionPage(parent), manager_(nullptr), port_(new util::PortBox(this)) {
  this->setTitle(tr("Step 1: Connect to a running Empathica Bluetooth server"));
  this->setSubTitle(tr(
	  "Please specify the port for the running Empathica Bluetooth server. It handles the connection to the E4."));

  auto *layout = new QFormLayout(this);
  layout->addRow(tr("Port"), port_);
  this->setLayout(layout);
}

RemoteConnectionPage::~RemoteConnectionPage() {
  this->_cleanUp();
}

void RemoteConnectionPage::initializePage() {
  ConnectionPage::initializePage();
  this->connectionButton()->setEnabled(true);
}

void RemoteConnectionPage::connect() {
  manager_ = new model::ConnectionManager(QHostAddress::LocalHost, port_->value(), this);

  QObject::connect(manager_, &model::ConnectionManager::finished, [this] {
	manager_->deleteLater();
	manager_ = nullptr;
  });

  QObject::connect(manager_, &model::ConnectionManager::connected, this, [this](model::Connection *connection) {
	QObject::connect(connection, &model::Connection::disconnecting, this, [this] {
	  this->_cleanUp();
	});
	this->setConnected(true);
  });

  QObject::connect(manager_, &model::ConnectionManager::connectionFailed, this, [this](const QString &error) {
	QMessageBox::warning(this, tr("Unable to establish connection"), error);
	this->setConnected(false);
  });

  manager_->start();
}

model::Connection *RemoteConnectionPage::connection() {
  return manager_ != nullptr && is_connected_ ? manager_->connection() : nullptr;
}

void RemoteConnectionPage::cleanupPage() {
  QWizardPage::cleanupPage();
  this->_cleanUp();
}

void RemoteConnectionPage::_cleanUp() {
  if (manager_ != nullptr) {
	// This will trigger 'finished', which sets manager_ to nullptr
	manager_->shutdown(100);
  }
}

}

