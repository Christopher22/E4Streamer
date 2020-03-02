//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMAND_H_
#define E4STREAMER_SRC_MODEL_COMMAND_H_

#include "Disconnectable.h"

#include <QStringList>

namespace e4streamer::model {
class Response;

class Command : public Disconnectable {
 Q_OBJECT

 public:
  [[nodiscard]] virtual QString rawCommand() const;
  virtual bool report(Response *response);
  [[nodiscard]] bool isSuitable(Response *response) const;
  void handleDisconnect() override;
 protected:
  Command(QString command, QStringList arguments, QObject *parent = nullptr);
  virtual void handleResponse(Response *status);

 signals:
  void failure(const QString &error);
  void success();

 private:
  QString command_;
  QStringList arguments_;
};
}

#endif //E4STREAMER_SRC_MODEL_COMMAND_H_
