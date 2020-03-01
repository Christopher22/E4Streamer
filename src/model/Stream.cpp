//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Stream.h"
#include "Connection.h"
#include "commands/Subscription.h"

namespace e4streamer::model {

Stream::Stream(Sample::Type type, Connection *connection, QObject *parent)
    : QObject(parent),
      connection_(nullptr),
      state_(State::Unconnected),
      type_(type),
      stream_(Stream::_generateStreamInfo(type)) {
  this->setConnection(connection);
}

Stream::~Stream() {
  this->unsubscribe();
}

bool Stream::subscribe() {
  if (state_ != State::Unconnected || connection_ == nullptr) {
    return false;
  }

  state_ = State::Subscribing;
  auto command = connection_->send<commands::Subscription>(type_, true, nullptr);
  QObject::connect(command.command(), &commands::Subscription::success, this, &Stream::_onCommandSuccess);
  QObject::connect(command.command(), &commands::Subscription::failure, this, &Stream::_onCommandFailure);
  return true;
}

bool Stream::unsubscribe() {
  if (state_ != State::Subscribed || connection_ == nullptr) {
    return false;
  }

  state_ = State::Unsubscribing;
  auto command = connection_->send<commands::Subscription>(type_, false, nullptr);
  QObject::connect(command.command(), &commands::Subscription::success, this, &Stream::_onCommandSuccess);
  QObject::connect(command.command(), &commands::Subscription::failure, this, &Stream::_onCommandFailure);
  return true;
}

void Stream::_onSampleArriving(const Sample &sample) {
  if (state_ != State::Subscribed || sample.type() != type_) {
    return;
  }

  // Send the data to Lab Streaming Layer
  stream_.push_sample(sample.data(), sample.timestamp());
}

void Stream::_onConnectionDisconnect() {
  // It is to late to unsubscribe
  connection_ = nullptr;
  state_ = State::Unconnected;
}

void Stream::_onCommandSuccess() {
  if (state_ == State::Subscribing) {
    state_ = State::Subscribed;
    emit this->subscribed();
  } else if (state_ == State::Unsubscribing) {
    state_ = State::Unconnected;
    emit this->unsubscribed();
  }
}

void Stream::_onCommandFailure(const QString &error) {
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
  if (connection_ != nullptr || connection == nullptr) {
    return false;
  }

  connection_ = connection;
  QObject::connect(connection_, &Connection::disconnected, this, &Stream::_onConnectionDisconnect);
  QObject::connect(connection_, &Connection::destroyed, this, &Stream::_onConnectionDisconnect);
  QObject::connect(connection_, &Connection::sample, this, &Stream::_onSampleArriving);
  return true;
}

}