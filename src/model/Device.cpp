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
  this->disconnectDevice();
}

bool Device::connectDevice() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::NotConnected) {
    return false;
  }

  qDebug("Connecting device...");
  state_ = State::Connecting;
  auto command = connection->send<commands::Connect>(this);
  QObject::connect(command.command(), &commands::Connect::success, this, &Device::_onSuccess);
  QObject::connect(command.command(), &commands::Connect::failure, this, &Device::_onFailure);
  return true;
}

bool Device::disconnectDevice() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::Connected) {
    return false;
  }

  qDebug("Disconnecting device...");
  state_ = State::Disconnecting;
  auto disconnect = connection->send<commands::Disconnect>(this, nullptr);
  QObject::connect(disconnect.command(), &commands::Disconnect::success, this, &Device::_onSuccess);
  QObject::connect(disconnect.command(), &commands::Disconnect::failure, this, &Device::_onFailure);
  return true;
}

QString Device::ToString() const {
  return tr("'%1' (%2)").arg(id_, name_.split('_').join(' '));
}

void Device::_onSuccess() {
  if (state_ == State::Connecting) {
    qDebug("Device successfully connected.");
    state_ = State::Connected;
    emit this->connected(this);
  } else if (state_ == State::Disconnecting) {
    qDebug("Device successfully disconnected.");
    state_ = State::NotConnected;
    emit this->disconnected(this);
  }
}

void Device::_onFailure(const QString &failure) {
  if (state_ == State::Connecting) {
    qWarning() << "Connecting device failed: " << failure;
    state_ = State::NotConnected;
    emit this->connectionFailed(failure);
  } else if (state_ == State::Disconnecting) {
    qWarning() << "Disconnecting device failed: " << failure;
    state_ = State::Connected;
    emit this->disconnectionFailed(failure);
  }
}

}