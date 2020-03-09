//
// Created by christopher on 26.02.2020.
//

#include "DiscoverDevices.h"
#include "../Connection.h"
#include "../Response.h"
#include "../Device.h"

namespace e4streamer::model::commands {

DiscoverDevices::DiscoverDevices(bool connect_manually, QObject *parent) : Command(QString(connect_manually
																						   ? "device_discover_list"
																						   : "device_list"),
																				   QStringList(),
																				   parent),
																		   connect_manually_(connect_manually) {}

void DiscoverDevices::handleResponse(Response *response) {
  if (response->empty()) {
	return;
  }

  bool is_int = false;
  const int num_strings_per_device = connect_manually_ ? 4 : 3;
  const int num_devices = (*response)[0].toInt(&is_int);
  const int expected_length = num_devices * num_strings_per_device + 1;
  if (!is_int || response->size() != expected_length) {
	return;
  }

  response->accept();
  QVector<Device *> devices;
  devices.reserve(num_devices);
  for (int i = 2; i < expected_length; i += num_strings_per_device) {
	const bool is_allowed = connect_manually_ ? (*response)[i + 2].compare("allowed", Qt::CaseInsensitive) == 0 : true;
	auto *device = new Device((*response)[i],
							  (*response)[i + 1],
							  is_allowed,
							  connect_manually_,
							  nullptr);
	response->connection()->addChild(device);
	devices.push_back(device);
  }

  qRegisterMetaType<QVector<Device *>>("QVector<Device*>");
  emit this->success(devices);
}

}