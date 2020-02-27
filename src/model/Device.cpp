//
// Created by christopher on 26.02.2020.
//

#include "Device.h"
#include "Connection.h"
#include "commands/Connect.h"
#include "commands/Disconnect.h"

namespace e4streamer::model {

Device::Device(QString id, QString name, bool is_allowed, Connection *connection)
	: QObject(connection),
	  id_(std::move(id)),
	  name_(std::move(name)),
	  is_allowed_(is_allowed),
	  state_(State::NotConnected) {

}

Device::~Device() {
  this->disconnect();
}

bool Device::connect() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::NotConnected) {
	return false;
  }

  state_ = State::Connecting;

  auto command = connection->send<commands::Connect>(this);
  QObject::connect(command.command(), &commands::Connect::success, [this] {
	state_ = State::Connected;
	emit this->connected();
  });
  QObject::connect(command.command(),
				   &commands::Connect::failure,
				   [this](const QString &error_msg) {
					 state_ = State::NotConnected;
					 emit this->connectionFailed(error_msg);
				   });

  return true;
}

bool Device::disconnect() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::Connected) {
	return false;
  }

  // In case of failure, we are indefined. Do not handle that.
  state_ = State::NotConnected;
  auto disconnect = connection->send<commands::Disconnect>(this, nullptr);
  QObject::connect(disconnect.command(), &commands::Connect::success, [this] {
	emit this->disconnected();
  });

  return true;
}
}