//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_SERVERCONFIGWIDGET_H_
#define E4STREAMER_SRC_VIEW_SERVERCONFIGWIDGET_H_

#include <QWidget>

namespace e4streamer {
namespace model {
class Server;
class Connection;
}

namespace view::widgets {
class ServerConfig : public QWidget {
 Q_OBJECT

 public:
  explicit ServerConfig(QWidget *parent = nullptr);
  [[nodiscard]] model::Connection *connection() const;
  [[nodiscard]] inline model::Server *model() noexcept {
	return server_;
  }

 private:
  void _onConnectionFailed(const QString &error);
  model::Server *server_;
};
}
}

#endif //E4STREAMER_SRC_VIEW_SERVERCONFIGWIDGET_H_
