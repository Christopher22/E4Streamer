//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_DEVICE_H_
#define E4STREAMER_SRC_MODEL_DEVICE_H_

#include "Command.h"

#include <QObject>

namespace e4streamer::model {
class Connection;

class Device : public QObject {
 Q_OBJECT

 public:
  enum class State {
    NotConnected,
    Connecting,
    Connected,
    Disconnecting
  };

  Device(QString id, QString name, bool is_allowed, Connection *connection = nullptr);
  ~Device() override;
  [[nodiscard]] QString ToString() const;

  bool connectDevice();
  bool disconnectDevice();

  [[nodiscard]] inline bool isConnectable() const {
    return is_allowed_;
  }

  [[nodiscard]] inline const QString &id() const {
    return id_;
  }

  [[nodiscard]] inline State state() const {
    return state_;
  }

 signals:
  void connected(Device *device);
  void disconnected(Device *device);
  void connectionFailed(const QString &error);
  void disconnectionFailed(const QString &error);

 private:
  void _onSuccess();
  void _onFailure(const QString &failure);

  const QString id_, name_;
  const bool is_allowed_;
  State state_;
};
}

#endif //E4STREAMER_SRC_MODEL_DEVICE_H_
