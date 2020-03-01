//
// Created by Christopher Gundler on 01.03.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_REGISTER_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_REGISTER_H_

#include "../Command.h"

namespace e4streamer::model {
class Device;

namespace commands {
class Register : public Command {
 public:
  explicit Register(Device *device, QObject *parent = nullptr);
};
}
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_REGISTER_H_
