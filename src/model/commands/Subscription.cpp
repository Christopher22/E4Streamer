//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Subscription.h"
#include "../Response.h"

namespace e4streamer::model::commands {

Subscription::Subscription(Sample::Type type, bool subscribe, QObject *parent) : Command("device_subscribe",
                                                                                         {Sample::id(type),
                                                                                          subscribe ? "ON" : "OFF"},
                                                                                         parent), type_(type) {

}

bool Subscription::report(Response *response) {
  if (response == nullptr || !this->isSuitable(response) || response->size() < 2
      || Sample::type((*response)[0].toLatin1()) != type_) {
    return false;
  }

  response->accept();
  if ((*response)[1] == "OK") {
    emit this->success();
  } else {
    response->pop_front();
    response->pop_front();
    emit this->failure(response->join(' '));
  }
  return true;
}
}