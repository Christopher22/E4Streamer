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
class ConnectionPage;
class DevicePage;
class RecordingPage;

class Wizard : public QWizard {
 Q_OBJECT

 public:
  explicit Wizard(QWidget *widget = nullptr);
  model::Connection *connection();

 private:
  ConnectionPage *connection_page_;
  DevicePage *device_page_;
  RecordingPage *recording_page_;
};
}
}

#endif //E4STREAMER_SRC_VIEW_WIZARD_H_
