//
// Created by Christopher Gundler on 01.03.2020.
//

#ifndef E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_
#define E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_

#include <QThread>

namespace e4streamer::model {
class Connection;

class ConnectionManager : public QThread {
 Q_OBJECT

 public:
  explicit ConnectionManager(QObject *parent = nullptr);
  void shutdown(unsigned long timeout);

 protected:
  void run() override;

 signals:
  void connectionCreated(Connection *connection);

 private:
  Connection *connection_;
};
}

#endif //E4STREAMER_SRC_MODEL_CONNECTIONMANAGER_H_
