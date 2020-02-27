//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_COMMAND_H_
#define E4STREAMER_SRC_MODEL_COMMAND_H_

#include <QObject>
#include <QStringList>

namespace e4streamer::model {
class Response;

class Command : public QObject {
 Q_OBJECT

 public:
  [[nodiscard]] virtual QString rawCommand() const;
  virtual bool report(Response *response);

 protected:
  Command(QString command, QStringList arguments, QObject *parent = nullptr);
  [[nodiscard]] bool isSuitable(Response *response) const;
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
