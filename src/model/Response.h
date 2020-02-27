//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_RESPONSE_H_
#define E4STREAMER_SRC_MODEL_RESPONSE_H_

#include <QString>
#include <QStringList>

namespace e4streamer::model {
class Response : public QStringList {
 public:
  static std::unique_ptr<Response> parse(const QString &raw_response);

  inline void accept() noexcept {
	is_accepted_ = true;
  }

  [[nodiscard]] inline bool is_accepted() const noexcept {
	return is_accepted_;
  }

  [[nodiscard]] inline const QString &command() const noexcept {
	return command_;
  }

 protected:
  explicit Response(QString response_target, QStringList &&arguments);

 private:
  QString command_;
  bool is_accepted_;
};
}

#endif //E4STREAMER_SRC_MODEL_RESPONSE_H_
