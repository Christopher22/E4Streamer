//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_WIZARD_H_
#define E4STREAMER_SRC_VIEW_WIZARD_H_

#include <QWizard>

namespace e4streamer::view {
class Wizard : public QWizard {
 public:
  static constexpr const char *CONNECTION = "connection";

  explicit Wizard(QWidget *widget = nullptr);
};
}

#endif //E4STREAMER_SRC_VIEW_WIZARD_H_
