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

Device::Device(QString id, QString name, bool is_allowed, bool connect_manually, Connection *connection)
	: Disconnectable(connection),
	  id_(std::move(id)),
	  name_(std::move(name)),
	  is_allowed_(is_allowed),
	  state_(State::NotConnected),
	  is_shutting_down_(false),
	  connect_manually_(connect_manually) {
}

Device::~Device() {
  if (state_ != State::NotConnected) {
	qWarning() << "Device" << id_ << "was not properly disconnected!";
  }
}

bool Device::connectDevice() {
  if (state_ != State::NotConnected || is_shutting_down_) {
	return false;
  }

  if (connect_manually_) {
	qDebug() << "Registering device" << id_;
	return this->_sendCommand<commands::Register>(State::Registering);
  } else {
	qDebug() << "Connecting device" << id_ << "directly";
	return this->_sendCommand<commands::Connect>(State::Connecting);
  }
}

bool Device::disconnectDevice() {
  if (state_ != State::Connected) {
	return false;
  }

  qDebug() << "Disconnecting device" << id_;
  return this->_sendCommand<commands::Disconnect>(State::Disconnecting);
}

void Device::handleDisconnect() {
  // Do not allow multiple calls
  if (is_shutting_down_) {
	return;
  }

  is_shutting_down_ = true;
  if (state_ != State::Connected) {
	this->deleteLater();
	return;
  }
  this->disconnectDevice();
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
	// An explicit unregister is not necessary even if 'device_connect_btle' was used.
	qDebug("Device successfully disconnected.");
	state_ = State::NotConnected;
	emit this->disconnected(this);

	// Success! The device was successfully disconnected and remove itself from connection.
	if (is_shutting_down_) {
	  this->deleteLater();
	}
  }
}

void Device::_onFailure(const QString &failure) {
  // During shutdown, a command failed. We have no way to react but simply delete this class.
  if (is_shutting_down_) {
	state_ = State::NotConnected;
	this->deleteLater();
	return;
  }

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

Connection *Device::connection() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (connection == nullptr && state_ != State::NotConnected) {
	this->_onFailure("Invalid parent of device: Not a connection!");
  }
  return connection;
}

}