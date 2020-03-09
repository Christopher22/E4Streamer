//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_SERVER_H_
#define E4STREAMER_SRC_MODEL_SERVER_H_

// Required for property
#include "Connection.h"

#include <QObject>
#include <QHostAddress>
#include <QIODevice>

class QThread;
class QProcess;

namespace e4streamer::model {
class ConnectionManager;

class Server : public QObject {
 Q_OBJECT

 public:
  enum class State {
	NotConnected,
	ServerStarting,
	Connecting,
	Connected
  };

  Q_PROPERTY(Connection *connection
				 READ
					 connection
				 NOTIFY
				 connected)
  Q_PROPERTY(bool ready
				 READ
					 isReady
				 NOTIFY
				 readinessChanged)

  explicit Server(QString server_path = QString(),
				  QString api_key = QString(),
				  quint16 port = 8000,
				  QObject *parent = nullptr);
  ~Server() override;
  bool start();
  [[nodiscard]] bool isReady() const noexcept;

  bool setServerPath(const QString &server_path);
  bool setApiKey(const QString &api_key);
  bool setPort(quint16 port);

  [[nodiscard]] inline quint16 port() const noexcept {
	return port_;
  }

  [[nodiscard]] inline QString apiKey() const noexcept {
	return api_key_;
  }

  [[nodiscard]] inline QString serverPath() const noexcept {
	return server_path_;
  }

  [[nodiscard]] inline Connection *connection() {
	return state_ == State::Connected ? connection_ : nullptr;
  }

  [[nodiscard]] inline State state() const {
	return state_;
  }

 signals:
  void readinessChanged(bool is_ready);
  void connectionFailed(const QString &error);
  void connected(Connection *connection);

 private:
  void _cleanUp();

  template<typename T, typename F>
  bool _set_value(const T &value, F setter) {
	if (state_ != State::NotConnected) {
	  return false;
	}

	const bool old_is_ready = this->isReady();
	setter(value);
	const bool new_is_ready = this->isReady();
	if (old_is_ready != new_is_ready) {
	  emit this->readinessChanged(new_is_ready);
	}
	return true;
  }

  QString server_path_, api_key_;
  quint16 port_;
  Connection *connection_;
  ConnectionManager *background_worker_;
  QProcess *background_server_;
  State state_;
};
}

#endif //E4STREAMER_SRC_MODEL_SERVER_H_
