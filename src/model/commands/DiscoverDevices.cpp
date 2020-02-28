//
// Created by christopher on 26.02.2020.
//

#include "DiscoverDevices.h"
#include "../Connection.h"
#include "../Response.h"
#include "../Device.h"

namespace e4streamer::model::commands {

DiscoverDevices::DiscoverDevices(QObject *parent) : Command("device_discover_list",
															QStringList(),
															parent) {}

void DiscoverDevices::handleResponse(Response *response) {
  if (response->empty()) {
	return;
  }

  bool is_int = false;
  int num_devices = (*response)[0].toInt(&is_int);
  if (!is_int || response->size() != num_devices * 4 + 1) {
	return;
  }

  response->accept();
  QVector<Device *> devices;
  devices.reserve(num_devices);
  for (int i = 1, size = num_devices * 4 + 1; i < size; i += 4) {
	auto *device = new Device((*response)[i],
							  (*response)[i + 1],
							  (*response)[i + 2].compare("allowed", Qt::CaseInsensitive),
							  nullptr);
	device->moveToThread(response->connection()->thread());
	device->setParent(response->connection());
	devices.push_back(device);
  }

  emit this->success(devices);
}

}