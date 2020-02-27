//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_SERVER_H_
#define E4STREAMER_SRC_MODEL_SERVER_H_

#include <QObject>
#include <QHostAddress>
#include <QIODevice>

class QThread;
class QProcess;

namespace e4streamer::model {
class Connection;

class Server : public QObject {
 Q_OBJECT

 public:
  enum class State {
	NotConnected,
	ServerStarting,
	Connecting,
	Connected
  };

  explicit Server(QString server_path = QString(),
				  QString api_key = QString(),
				  quint16 port = 8000,
				  QObject *parent = nullptr);
  ~Server() override;
  bool start();

  inline void setPort(quint16 port) {
	port_ = port;
  }

  [[nodiscard]] inline quint16 port() const noexcept {
	return port_;
  }

  inline void setApiKey(const QString &api_key) {
	api_key_ = api_key;
  }

  [[nodiscard]] inline QString apiKey() const noexcept {
	return api_key_;
  }

  inline void setServerPath(const QString &server_path) {
	server_path_ = server_path;
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
  void connecting(const QHostAddress &address, quint16 port, QIODevice::OpenMode mode);
  void connectionFailed(const QString &error);
  void connected(Connection *connection);

 private:
  void _cleanUp();

  QString server_path_, api_key_;
  quint16 port_;
  Connection *connection_;
  QThread *background_worker_;
  QProcess *background_server_;
  State state_;
};
}

#endif //E4STREAMER_SRC_MODEL_SERVER_H_
