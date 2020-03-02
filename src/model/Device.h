//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_DEVICE_H_
#define E4STREAMER_SRC_MODEL_DEVICE_H_

#include "Disconnectable.h"
#include "Command.h"

namespace e4streamer::model {
class Connection;

class Device : public Disconnectable {
 Q_OBJECT

 public:
  enum class State {
    NotConnected,
    Registering,
    Connecting,
    Connected,
    Disconnecting,
    Unregistering
  };

  Device(QString id, QString name, bool is_allowed, Connection *connection = nullptr);
  ~Device() override;
  [[nodiscard]] QString ToString() const;
  bool connectDevice();
  bool disconnectDevice();
  void handleDisconnect() override;

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
  Connection *connection();
  void _onSuccess();
  void _onFailure(const QString &failure);

  template<typename T>
  bool _sendCommand(State next_state) {
    auto *connection = this->connection();
    if (connection == nullptr) {
      return false;
    }

    state_ = next_state;
    auto connect = connection->send<T>(this, nullptr);
    QObject::connect(connect.command(), &T::success, this, &Device::_onSuccess);
    QObject::connect(connect.command(), &T::failure, this, &Device::_onFailure);
    return true;
  }

  const QString id_, name_;
  const bool is_allowed_;
  State state_;
  bool is_shutting_down_;
};
}

#endif //E4STREAMER_SRC_MODEL_DEVICE_H_
