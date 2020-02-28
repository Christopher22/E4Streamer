//
// Created by christopher on 26.02.2020.
//

#include "Command.h"
#include "Response.h"

#include <QDebug>

namespace e4streamer::model {

Command::Command(QString command, QStringList arguments, QObject *parent)
	: QObject(parent), command_(std::move(command)), arguments_(std::move(arguments)) {

}

QString Command::rawCommand() const {
  return QString("%1 %2").arg(command_, arguments_.join(' '));
}

bool Command::isSuitable(Response *response) const {
  return response->command().compare(this->command_, Qt::CaseInsensitive) == 0;
}

bool Command::report(Response *response) {
  if (response == nullptr || !this->isSuitable(response)) {
	return false;
  }

  if (!response->isEmpty() && (*response)[0].compare("ERR", Qt::CaseInsensitive) == 0) {
	response->accept();
	response->pop_front();
	emit this->failure(response->join(' '));
  } else if (!response->isEmpty() && (*response)[0].compare("OK", Qt::CaseInsensitive) == 0) {
	response->accept();
	emit this->success();
  } else {
	this->handleResponse(response);
  }

  return response->is_accepted();
}

void Command::handleResponse(Response *) {}
}