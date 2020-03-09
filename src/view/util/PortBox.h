//
// Created by christopher on 09.03.2020.
//

#ifndef E4STREAMER_SRC_VIEW_UTIL_PORTBOX_H_
#define E4STREAMER_SRC_VIEW_UTIL_PORTBOX_H_

#include <QSpinBox>

namespace e4streamer::view::util {
class PortBox : public QSpinBox {
 public:
  explicit PortBox(QWidget *parent = nullptr);
};
}

#endif //E4STREAMER_SRC_VIEW_UTIL_PORTBOX_H_
