//
// Created by Christopher Gundler on 29.02.2020.
//

#include "StreamWidget.h"
#include "../../model/Stream.h"

#include <QMessageBox>

namespace e4streamer::view::widgets {

StreamWidget::StreamWidget(model::Sample::Type type, QWidget *parent) :
    QCheckBox(model::Sample::description(type), parent),
    stream_(new model::Stream(type, nullptr, this)),
    ignore_next_state_change_(false) {
  this->setEnabled(false);

  QObject::connect(this, &StreamWidget::stateChanged, this, &StreamWidget::_onStateChange);
  QObject::connect(stream_, &model::Stream::subscribed, this, &StreamWidget::_onSuccess);
  QObject::connect(stream_, &model::Stream::unsubscribed, this, &StreamWidget::_onSuccess);
  QObject::connect(stream_, &model::Stream::subscriptionFailed, this, &StreamWidget::_onFailure);
  QObject::connect(stream_, &model::Stream::unsubscriptionFailed, this, &StreamWidget::_onFailure);
}

void StreamWidget::setConnection(model::Connection *connection) {
  this->setEnabled(stream_->setConnection(connection));
}

void StreamWidget::_onStateChange(int new_state) {
  // A hack for avoiding the danger of recursive calling
  if (ignore_next_state_change_) {
    ignore_next_state_change_ = false;
    return;
  }

  if (new_state == Qt::Checked && stream_->state() == model::Stream::State::Unconnected) {
    this->setEnabled(false);
    stream_->subscribe();
  } else if (new_state == Qt::Unchecked && stream_->state() == model::Stream::State::Subscribed) {
    this->setEnabled(false);
    stream_->unsubscribe();
  }
}
void StreamWidget::_onSuccess() {
  this->setEnabled(true);
}

void StreamWidget::_onFailure(const QString &error) {
  QMessageBox::warning(this, tr("Subscription failed"), error);
  ignore_next_state_change_ = true;
  this->setChecked(!this->isChecked());
  this->setEnabled(true);
}

}