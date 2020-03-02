//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Stream.h"
#include "Connection.h"
#include "commands/Subscription.h"

namespace e4streamer::model {

Stream::Stream(Sample::Type type, Connection *connection, Connection *parent)
    : Disconnectable(parent),
      state_(State::Unconnected),
      type_(type),
      stream_(Stream::_generateStreamInfo(type)),
      is_shutting_down_(false) {
  this->setConnection(connection);
}

Stream::~Stream() {
  if (!is_shutting_down_) {
    is_shutting_down_ = true;
    this->unsubscribe();
  }
}

bool Stream::subscribe() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (state_ != State::Unconnected || connection == nullptr) {
    return false;
  }

  state_ = State::Subscribing;
  auto command = connection->send<commands::Subscription>(type_, true, nullptr);
  QObject::connect(command.command(), &commands::Subscription::success, this, &Stream::_onCommandSuccess);
  QObject::connect(command.command(), &commands::Subscription::failure, this, &Stream::_onCommandFailure);
  return true;
}

bool Stream::unsubscribe() {
  auto *connection = qobject_cast<Connection *>(this->parent());
  if (state_ != State::Subscribed || connection == nullptr) {
    return false;
  }

  state_ = State::Unsubscribing;
  auto command = connection->send<commands::Subscription>(type_, false, nullptr);
  QObject::connect(command.command(), &commands::Subscription::success, this, &Stream::_onCommandSuccess);
  QObject::connect(command.command(), &commands::Subscription::failure, this, &Stream::_onCommandFailure);
  return true;
}

void Stream::handleDisconnect() {
  if (is_shutting_down_) {
    return;
  }

  is_shutting_down_ = true;
  if (!this->unsubscribe()) {
    this->deleteLater();
    return;
  }
}

void Stream::_onSampleArriving(const Sample &sample) {
  if (state_ != State::Subscribed || sample.type() != type_) {
    return;
  }

  // Send the data to Lab Streaming Layer
  stream_.push_sample(sample.data(), sample.timestamp());
}

void Stream::_onCommandSuccess() {
  if (state_ == State::Subscribing) {
    state_ = State::Subscribed;
    emit this->subscribed();
  } else if (state_ == State::Unsubscribing) {
    state_ = State::Unconnected;
    emit this->unsubscribed();

    // Successfully shut down
    if (is_shutting_down_) {
      this->deleteLater();
    }
  }
}

void Stream::_onCommandFailure(const QString &error) {
  // We can do nothing - do not throw error.
  if (is_shutting_down_) {
    state_ = State::Unconnected;
    this->deleteLater();
    return;
  }

  if (state_ == State::Subscribing) {
    state_ = State::Unconnected;
    emit this->subscriptionFailed(error);
  } else if (state_ == State::Unsubscribing) {
    state_ = State::Subscribed;
    emit this->unsubscriptionFailed(error);
  }
}

lsl::stream_info Stream::_generateStreamInfo(Sample::Type sample_type) {
  QString description(Sample::description(sample_type));
  return lsl::stream_info(tr("Empathica E4: %1").arg(description).toStdString(),
                          description.toStdString(),
                          Sample::numChannels(sample_type),
                          lsl::IRREGULAR_RATE);
}

bool Stream::setConnection(Connection *connection) {
  if (connection == nullptr || connection == this->parent()) {
    return false;
  }

  connection->addChild(this);
  QObject::connect(connection, &Connection::sample, this, &Stream::_onSampleArriving);
  return true;
}

}