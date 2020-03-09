//
// Created by christopher on 09.03.2020.
//

#ifndef E4STREAMER_SRC_VIEW_CONNECTIONPAGE_H_
#define E4STREAMER_SRC_VIEW_CONNECTIONPAGE_H_

#include <QWizardPage>

namespace e4streamer {

namespace model {
class Connection;
}

namespace view {
class ConnectionPage : public QWizardPage {
 public:
  explicit ConnectionPage(QWidget *parent = nullptr);
  virtual model::Connection *connection() = 0;
  void initializePage() override;
  bool validatePage() override;
  [[nodiscard]] bool isComplete() const override;

 protected:
  virtual void connect() = 0;
  void setConnected(bool is_connected);

  bool is_connected_{};
};
}
}

#endif //E4STREAMER_SRC_VIEW_CONNECTIONPAGE_H_
