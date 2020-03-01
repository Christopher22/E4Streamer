//
// Created by Christopher Gundler on 29.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_STREAM_H_
#define E4STREAMER_SRC_MODEL_STREAM_H_

#include "Sample.h"

#include <QObject>
#include <lsl_cpp.h>

namespace e4streamer::model {
class Connection;

class Stream : public QObject {
 Q_OBJECT

 public:
  enum class State {
    Unconnected,
    Subscribing,
    Subscribed,
    Unsubscribing
  };

  explicit Stream(Sample::Type type, Connection *connection = nullptr, QObject *parent = nullptr);
  ~Stream() override;

  bool setConnection(Connection *connection);
  bool subscribe();
  bool unsubscribe();
  [[nodiscard]] inline State state() const noexcept {
    return state_;
  }

 signals:
  void subscribed();
  void subscriptionFailed(const QString &error);
  void unsubscribed();
  void unsubscriptionFailed(const QString &error);

 private:
  static lsl::stream_info _generateStreamInfo(Sample::Type sample_type);
  void _onCommandSuccess();
  void _onCommandFailure(const QString &error);
  void _onSampleArriving(const Sample &sample);
  void _onConnectionDisconnect();

  Connection *connection_;
  State state_;
  Sample::Type type_;
  lsl::stream_outlet stream_;
};
}

#endif //E4STREAMER_SRC_MODEL_STREAM_H_
