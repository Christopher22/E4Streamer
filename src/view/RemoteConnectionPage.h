//
// Created by christopher on 09.03.2020.
//

#ifndef E4STREAMER_SRC_VIEW_REMOTECONNECTIONPAGE_H_
#define E4STREAMER_SRC_VIEW_REMOTECONNECTIONPAGE_H_

#include "ConnectionPage.h"

namespace e4streamer {
namespace model {
class Connection;
class ConnectionManager;
}

namespace view {
namespace util {
class PortBox;
}

class RemoteConnectionPage : public ConnectionPage {
 public:
  explicit RemoteConnectionPage(QWidget *parent = nullptr);
  ~RemoteConnectionPage() override;
  void initializePage() override;
  void cleanupPage() override;
  model::Connection *connection() override;

 protected:
  void connect() override;

 private:
  model::ConnectionManager *manager_;
  util::PortBox *port_;

  void _cleanUp();
};
}
}

#endif //E4STREAMER_SRC_VIEW_REMOTECONNECTIONPAGE_H_
