//
// Created by Christopher Gundler on 01.03.2020.
//

#include "Unregister.h"
#include "../Device.h"

namespace e4streamer::model::commands {

Unregister::Unregister(Device *device, QObject *parent) : Command("device_disconnect_btle", {device->id()}, parent) {}

}