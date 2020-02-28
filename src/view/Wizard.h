//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_WIZARD_H_
#define E4STREAMER_SRC_VIEW_WIZARD_H_

#include <QWizard>

namespace e4streamer {
namespace model {
class Connection;
class Device;
}

namespace view {
class ServerPage;
class DevicePage;

class Wizard : public QWizard {
 Q_OBJECT

 public:
  explicit Wizard(QWidget *widget = nullptr);
  model::Connection *connection();
  model::Device *device();

 private:
  ServerPage *server_page_;
  DevicePage *device_page_;
};
}
}

#endif //E4STREAMER_SRC_VIEW_WIZARD_H_
