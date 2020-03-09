//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_SERVERPAGE_H_
#define E4STREAMER_SRC_VIEW_SERVERPAGE_H_

#include "ConnectionPage.h"

namespace e4streamer::view {
namespace widgets {
class ServerConfig;
}

class ServerPage : public ConnectionPage {
 public:
  explicit ServerPage(QWidget *parent = nullptr);
  void initializePage() override;
  model::Connection *connection() override;

 protected:
  void connect() override;

 private:
  widgets::ServerConfig *server_config_;
};
}

#endif //E4STREAMER_SRC_VIEW_SERVERPAGE_H_
