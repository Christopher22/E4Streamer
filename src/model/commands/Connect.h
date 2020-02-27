//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_CONNECT_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_CONNECT_H_

#include "../Command.h"

namespace e4streamer::model {
class Device;

namespace commands {
class Connect : public Command {
 public:
  explicit Connect(Device *device, QObject *parent = nullptr);
};
}
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_CONNECT_H_
