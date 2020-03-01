//
// Created by Christopher Gundler on 29.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_WIDGETS_STREAMWIDGET_H_
#define E4STREAMER_SRC_VIEW_WIDGETS_STREAMWIDGET_H_

#include "../../model/Sample.h"

#include <QCheckBox>

namespace e4streamer {
namespace model {
class Stream;
class Connection;
}

namespace view::widgets {
class StreamWidget : public QCheckBox {
 public:
  explicit StreamWidget(model::Sample::Type type, QWidget *parent = nullptr);
  void setConnection(model::Connection *connection);

 private:
  void _onStateChange(int state);
  void _onSuccess();
  void _onFailure(const QString &error);

  model::Stream *stream_;
  bool ignore_next_state_change_;
};
}
}

#endif //E4STREAMER_SRC_VIEW_WIDGETS_STREAMWIDGET_H_
