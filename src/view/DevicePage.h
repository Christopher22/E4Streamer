//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_DEVICEPAGE_H_
#define E4STREAMER_SRC_VIEW_DEVICEPAGE_H_

#include <QWizardPage>

namespace e4streamer::view {
namespace widgets {
class Devices;
}

class DevicePage : public QWizardPage {
 public:
  explicit DevicePage(QWidget *parent = nullptr);
  void initializePage() override;
  bool validatePage() override;
  [[nodiscard]] bool isComplete() const override;

 private:
  widgets::Devices *devices_;
};
}

#endif //E4STREAMER_SRC_VIEW_DEVICEPAGE_H_
