//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_DISCONNECT_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_DISCONNECT_H_

#include "../Command.h"

namespace e4streamer::model {
class Device;

namespace commands {
class Disconnect : public Command {
 public:
  explicit Disconnect(Device *device, QObject *parent = nullptr);
};
}
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_DISCONNECT_H_
