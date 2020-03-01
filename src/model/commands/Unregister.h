//
// Created by Christopher Gundler on 01.03.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_UNREGISTER_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_UNREGISTER_H_

#include "../Command.h"

namespace e4streamer::model {
class Device;

namespace commands {
class Unregister : public Command {
 public:
  explicit Unregister(Device *device, QObject *parent = nullptr);
};
}
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_UNREGISTER_H_
