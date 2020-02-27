//
// Created by christopher on 26.02.2020.
//

#include "Connection.h"
#include "Command.h"
#include "Response.h"

#include <QtDebug>

namespace e4streamer::model {

Connection::Connection(QObject *parent) : QTcpSocket(parent) {
  QObject::connect(this, &QTcpSocket::readyRead, this, &Connection::_processReceivedData);
}

void Connection::writeLine(const QString &line) {
  this->write(line.toUtf8());
  this->write("\r\n");
}

void Connection::_processReceivedData() {
  while (this->canReadLine()) {
	const QString received_string = QString::fromUtf8(this->readLine());

	if (received_string.startsWith('R')) {
	  // Try to parse the response
	  std::unique_ptr<Response> response = Response::parse(received_string);
	  if (!response) {
		qWarning() << "The server was unable to parse the following response:" << received_string;
	  }

	  // Find the corresponding command to response and execute it.
	  for (int i = 0, size = this->commands_.size(); i < size; ++i) {
		commands_[i]->report(response.get());
		if (response->is_accepted()) {
		  commands_[i]->deleteLater();
		  commands_.remove(i);
		  break;
		}
	  }

	  // TODO: Handle status data of devices
	  if (!response->is_accepted()) {
		qWarning() << "The server was unable to find a corresponding command to response " << response->command();
	  }
	} else {
	  // TODO: Handle streamed data
	  qWarning() << "The server was unable to parse the following response:" << received_string;
	}
  }

  // Ensure that no data is left.
  if (this->bytesAvailable() > 0) {
	qWarning() << this->bytesAvailable() << " bytes were discarded.";
  }
}

}