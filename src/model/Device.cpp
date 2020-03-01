//
// Created by christopher on 26.02.2020.
//

#include <QtCore/qcoreevent.h>
#include <src/model/commands/Register.h>
#include <src/model/commands/Unregister.h>
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

  QObject::connect(this, &Device::connectionSet, [this](Connection *connection) {
    qDebug("Setting auto device-disconnect for device");
    QObject::disconnect(nullptr, &Connection::disconnecting, this, &Device::disconnectDevice);
    if (connection != nullptr) {
      QObject::connect(connection, &Connection::disconnecting, this, &Device::disconnectDevice);
    }
  });

  if (connection != nullptr) {
    emit this->connectionSet(connection);
  }
}

Device::~Device() {
  this->disconnectDevice();
}

bool Device::connectDevice() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::NotConnected) {
    return false;
  }

  qDebug() << "Registering device" << id_;
  state_ = State::Registering;
  auto command = connection->send<commands::Register>(this);
  QObject::connect(command.command(), &commands::Register::success, this, &Device::_onSuccess);
  QObject::connect(command.command(), &commands::Register::failure, this, &Device::_onFailure);
  return true;
}

bool Device::disconnectDevice() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr || state_ != State::Connected) {
    return false;
  }

  qDebug() << "Disconnecting device" << id_;
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
  if (state_ == State::Registering) {
    qDebug("Device successfully registered. Connecting...");
    this->_sendCommand<commands::Connect>(State::Connecting);
  } else if (state_ == State::Connecting) {
    qDebug("Device successfully connected.");
    state_ = State::Connected;
    emit this->connected(this);
  } else if (state_ == State::Disconnecting) {
    qDebug("Device successfully disconnected. Unregistering...");
    this->_sendCommand<commands::Unregister>(State::Unregistering);
  } else if (state_ == State::Unregistering) {
    qDebug("Device successfully unregistered.");
    state_ = State::NotConnected;
    emit this->disconnected(this);
  }
}

void Device::_onFailure(const QString &failure) {
  if (state_ == State::Registering) {
    qWarning() << "Registering device failed: " << failure;
    state_ = State::NotConnected;
    emit this->connectionFailed(failure);
  } else if (state_ == State::Connecting) {
    qWarning() << "Connecting device failed: " << failure;
    // Registering was successful: Therefore, unregister.
    state_ = State::Unregistering;
    this->_sendCommand<commands::Unregister>(State::Unregistering);
    emit this->connectionFailed(failure);
  } else if (state_ == State::Disconnecting) {
    qWarning() << "Disconnecting device failed: " << failure;
    state_ = State::Connected;
    emit this->disconnectionFailed(failure);
  } else if (state_ == State::Unregistering) {
    qWarning() << "Unregistering device failed: " << failure;
    // We were unable to unregister while we were able to disconnect... We cannot undo the disconnection, now just hope this works.
    state_ = State::NotConnected;
    emit this->disconnectionFailed(failure);
  }
}

bool Device::event(QEvent *event) {
  if (event->type() == QEvent::Type::ParentChange) {
    emit this->connectionSet(qobject_cast<Connection *>(this->parent()));
  }
  return QObject::event(event);
}

Connection *Device::connection() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr && state_ != State::NotConnected) {
    this->_onFailure("Invalid parent of device: Not a connection!");
  }
  return connection;
}

}