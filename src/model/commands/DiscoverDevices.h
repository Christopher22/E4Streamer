//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMANDS_DISCOVERDEVICES_H_
#define E4STREAMER_SRC_MODEL_COMMANDS_DISCOVERDEVICES_H_

#include "../Command.h"
#include <QVector>

namespace e4streamer::model {
class Device;
class Connection;

namespace commands {
class DiscoverDevices : public Command {
 Q_OBJECT

 public:
  explicit DiscoverDevices(Connection *connection, QObject *parent = nullptr);

 signals:
  void success(const QVector<Device *> &devices);

 protected:
  void handleResponse(Response *response) override;

 private:
  Connection *connection_;
};
}
}

#endif //E4STREAMER_SRC_MODEL_COMMANDS_DISCOVERDEVICES_H_
