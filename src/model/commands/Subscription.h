//
// Created by Christopher Gundler on 29.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_SUBSCRIPTION_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_SUBSCRIPTION_H_

#include "../Command.h"
#include "../Sample.h"

namespace e4streamer::model::commands {
class Subscription : public Command {
 public:
  Subscription(Sample::Type type, bool subscribe, QObject *parent);
  bool report(Response *response) override;

 private:
  Sample::Type type_;
};
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_SUBSCRIPTION_H_
