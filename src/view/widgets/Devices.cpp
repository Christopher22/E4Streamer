//
// Created by christopher on 27.02.2020.
//

#include "Devices.h"
#include "../../model/Device.h"
#include "../../model/Connection.h"
#include "../../model/commands/DiscoverDevices.h"

#include <QMessageBox>
#include <QShortcut>

namespace e4streamer::view::widgets {

Devices::Devices(QWidget *parent)
	: QListWidget(parent), connection_(nullptr), selected_device_(nullptr) {
  auto *shortcut = new QShortcut(QKeySequence::Refresh, this);
  QObject::connect(shortcut, &QShortcut::activated, this, &Devices::updateDevices);
  QObject::connect(this, &Devices::itemDoubleClicked, this, &Devices::_selectDevice);
}

bool Devices::updateDevices() {
  if (!this->isEnabled() || connection_ == nullptr) {
	return false;
  }

  this->_clearDevices();
  this->setEnabled(false);

  auto command = connection_->send<model::commands::DiscoverDevices>();
  QObject::connect(command.command(), &model::commands::DiscoverDevices::success, this, &Devices::_updateDevices);
  QObject::connect(command.command(), &model::commands::DiscoverDevices::failure, this, &Devices::_handleUpdateFailure);

  return true;
}

bool Devices::setConnection(model::Connection *connection) {
  if (!this->isEnabled()) {
	return false;
  }

  this->_clearDevices();
  connection_ = connection;
  return true;
}

void Devices::_clearDevices() {
  if (!this->isEnabled()) {
	return;
  }

  this->setSelectedDevice(nullptr);

  for (int i = 0, size = this->count(); i < size; ++i) {
	auto *device = this->item(i)->data(Qt::UserRole).value<model::Device *>();
	connection_->removeChild(device);
  }
  this->clear();
}

void Devices::_updateDevices(const QVector<model::Device *> &devices) {
  for (auto *device: devices) {
	auto item = new QListWidgetItem(device->ToString(), this);
	item->setData(Qt::UserRole, QVariant::fromValue(device));
	item->setFlags(Qt::ItemNeverHasChildren);
	if (device->isConnectable()) {
	  item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
  }

  this->setEnabled(true);
}

model::Device *Devices::selectedDevice() const {
  return selected_device_;
}

void Devices::_selectDevice(QListWidgetItem *item) {
  auto *device = item->data(Qt::UserRole).value<model::Device *>();
  if (device == nullptr || !this->isEnabled()) {
	return;
  }

  this->setEnabled(false);

  // If a device is selected, deselect it first
  if (selected_device_ != nullptr) {
	QObject::connect(selected_device_,
					 &model::Device::disconnected,
					 [this, device](model::Device *disconnected_device) {
					   QObject::disconnect(disconnected_device, nullptr, this, nullptr);
					   this->_connectDevice(device);
					 });

	QObject::connect(selected_device_,
					 &model::Device::disconnectionFailed,
					 this,
					 &Devices::_handleDisconnectionFailure);
	selected_device_->disconnectDevice();
  } else {
	this->_connectDevice(device);
  }
}

void Devices::_connectDevice(model::Device *device) {
  QObject::connect(device, &model::Device::connected, this, &Devices::_onSelectedDevice);
  QObject::connect(device, &model::Device::connectionFailed, this, &Devices::_handleConnectionFailure);
  device->connectDevice();
}

void Devices::_onSelectedDevice(model::Device *device) {
  this->setEnabled(true);
  QObject::disconnect(device, nullptr, this, nullptr);
  this->setSelectedDevice(device);
}

void Devices::_handleUpdateFailure(const QString &error) {
  QMessageBox::warning(this, tr("Unable to query devices"), error);
  this->setEnabled(true);
}

void Devices::_handleConnectionFailure(const QString &error) {
  QMessageBox::warning(this, tr("Unable to connect to device"), error);
  this->setEnabled(true);
}

void Devices::_handleDisconnectionFailure(const QString &error) {
  QMessageBox::warning(this, tr("Unable to disconnect from device"), error);
  this->setEnabled(true);
}

void Devices::setSelectedDevice(model::Device *connected_device) {
// Update the current device
  const model::Device *old_device = selected_device_;
  selected_device_ = connected_device;
  if (connected_device != old_device) {
	emit this->deviceSelected(connected_device);
  }
}

}