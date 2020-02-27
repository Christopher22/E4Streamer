//
// Created by christopher on 26.02.2020.
//

#include "Command.h"
#include "Response.h"

namespace e4streamer::model {

Command::Command(QString command, QStringList arguments, QObject *parent)
	: QObject(parent), command_(std::move(command)), arguments_(std::move(arguments)) {

}

QString Command::rawCommand() const {
  return QString("%1 %2").arg(command_, arguments_.join(' '));
}

bool Command::isSuitable(Response *response) const {
  return response->command().compare(this->command_, Qt::CaseInsensitive);
}

bool Command::report(Response *response) {
  if (response == nullptr || !this->isSuitable(response)) {
	return false;
  }

  if (response->length() > 1 && (*response)[0].compare("ERR", Qt::CaseInsensitive)) {
	response->accept();
	response->pop_front();
	emit this->failure(response->join(' '));
  } else if (response->length() > 1 && (*response)[0].compare("OK", Qt::CaseInsensitive)) {
	response->accept();
	emit this->success();
  } else {
	this->handleResponse(response);
  }

  return response->is_accepted();
}

void Command::handleResponse(Response *) {

}

}