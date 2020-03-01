//
// Created by christopher on 26.02.2020.
//

#include "Disconnect.h"
#include "../Device.h"

namespace e4streamer::model::commands {

Disconnect::Disconnect(Device *device, QObject *parent) : Command("device_disconnect", {device->id()}, parent) {}

}