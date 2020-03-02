//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_SERVERPAGE_H_
#define E4STREAMER_SRC_VIEW_SERVERPAGE_H_

#include <QWizardPage>

namespace e4streamer {
namespace model {
class Connection;
}

namespace view {
namespace widgets {
class ServerConfig;
}

class ServerPage : public QWizardPage {
 public:
  explicit ServerPage(QWidget *parent = nullptr);
  void initializePage() override;
  bool validatePage() override;
  [[nodiscard]] bool isComplete() const override;
  model::Connection *connection();

 private:
  widgets::ServerConfig *server_config_;
};
}
}

#endif //E4STREAMER_SRC_VIEW_SERVERPAGE_H_
