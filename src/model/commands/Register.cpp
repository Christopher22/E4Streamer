//
// Created by Christopher Gundler on 01.03.2020.
//

#include "Register.h"
#include "../Device.h"

namespace e4streamer::model::commands {

Register::Register(Device *device, QObject *parent) : Command("device_connect_btle", {device->id()}, parent) {}

}