//
// Created by christopher on 26.02.2020.
//

#include "Connect.h"
#include "../Device.h"

namespace e4streamer::model::commands {

Connect::Connect(Device *device, QObject *parent) : Command("device_connect_btle", {device->id()}, parent) {}

}