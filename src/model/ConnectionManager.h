//
// Created by Christopher Gundler on 01.03.2020.
//

#ifndef E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_
#define E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_

#include "Connection.h"

#include <QThread>
#include <QtNetwork/QHostAddress>

namespace e4streamer::model {
class ConnectionManager : public QThread {
 Q_OBJECT

 public:
  explicit ConnectionManager(const QHostAddress &address, quint16 port, QObject *parent = nullptr);
  void shutdown(unsigned long timeout);
  [[nodiscard]] inline Connection *connection() noexcept {
	return connection_;
  }

 protected:
  void run() override;

 signals:
  void connected(Connection *connection);
  void connectionFailed(const QString &error);

 private:
  Connection *connection_;
  QHostAddress address_;
  quint16 port_;
};
}

#endif //E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_
