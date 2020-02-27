//
// Created by christopher on 27.02.2020.
//

#include "ServerConfig.h"
#include "../../model/Server.h"
#include "../../model/Connection.h"
#include "../util/FileSelector.h"

#include <QSpinBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QFormLayout>
#include <QPushButton>

#include <limits>

namespace e4streamer::view::widgets {

ServerConfig::ServerConfig(QWidget *parent) : QWidget(parent), server_(new model::Server()) {
  QObject::connect(server_, &model::Server::connectionFailed, this, &ServerConfig::_onConnectionFailed);

  auto *server_path = new util::FileSelector(tr("Please select the Empatica server"),
											 "Server (EmpaticaBLEServer.exe)",
											 false,
											 QString(),
											 this);
  QObject::connect(server_path, &util::FileSelector::pathSelected, server_, &model::Server::setServerPath);

  auto *port = new QSpinBox(this);
  port->setRange(1024, std::numeric_limits<quint16>::max());
  port->setValue(server_->port());
  QObject::connect(port, qOverload<int>(&QSpinBox::valueChanged), [&](int value) {
	server_->setPort(value);
  });

  auto *api_key = new QLineEdit(nullptr);
  api_key->setEchoMode(QLineEdit::Password);
  QObject::connect(api_key, &QLineEdit::textEdited, server_, &model::Server::setApiKey);

  auto *layout = new QFormLayout(this);
  layout->addRow(tr("Proprietary Empatica server: "), server_path);
  layout->addRow(tr("API key: "), api_key);
  layout->addRow(tr("Port: "), port);
  this->setLayout(layout);
}

void ServerConfig::_onConnectionFailed(const QString &error) {
  QMessageBox::warning(nullptr, "Unable to establish connection", error);
}

model::Connection *ServerConfig::connection() const {
  return server_->connection();
}

}