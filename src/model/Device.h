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
	Connected
  };

  Device(QString id, QString name, bool is_allowed, Connection *connection);
  ~Device() override;

  bool connect();
  bool disconnect();

  [[nodiscard]] inline const QString &id() const {
	return id_;
  }

  [[nodiscard]] inline State state() const {
	return state_;
  }

 signals:
  void connected();
  void disconnected();
  void connectionFailed(const QString &error);

 private:
  const QString id_, name_;
  const bool is_allowed_;
  State state_;
};
}

#endif //E4STREAMER_SRC_MODEL_DEVICE_H_
