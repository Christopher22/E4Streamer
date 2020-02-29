//
// Created by christopher on 26.02.2020.
//

#include "Connection.h"
#include "Command.h"
#include "Response.h"

#include <QtDebug>
#include <QThread>

namespace e4streamer::model {

Connection::Connection(QObject *parent) : QTcpSocket(parent) {
  QObject::connect(this, &QTcpSocket::readyRead, this, &Connection::_processReceivedData);
}

Connection::~Connection() {
  this->disconnectFromEmpathica();
}

void Connection::_writeLine(const QString &line) {
  this->write(line.toUtf8());
  this->write("\r\n");
}

void Connection::_processReceivedData() {
  while (this->canReadLine()) {
    const QString received_string = QString::fromUtf8(this->readLine());

    if (received_string.startsWith('R')) {
      // Try to parse the response
      std::unique_ptr<Response> response = Response::parse(this, received_string);
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
      const Sample sample = Sample::parse(received_string);
      if (sample) {
        emit this->sample(sample);
      } else {
        qWarning() << "The server was unable to parse the following data:" << received_string;
      }
    }
  }

  // Ensure that no data is left.
  if (this->bytesAvailable() > 0) {
    qWarning() << this->bytesAvailable() << " bytes were discarded.";
  }
}

void Connection::disconnectFromEmpathica() {
  if (this->state() != SocketState::ConnectingState && this->state() != SocketState::ConnectedState) {
    return;
  }

  this->disconnectFromHost();
  if (!this->waitForDisconnected(1000)) {
    qWarning() << "Unable to disconnect from socket: " << this->errorString();
  }
}

void Connection::registerCommand(Command *command) {
  Q_ASSERT(command->thread() == this->thread());
  Q_ASSERT(command->parent() == nullptr);

  command->setParent(this);
  commands_.push_back(command);
  this->_writeLine(command->rawCommand());
}

bool Connection::addChild(QObject *child) {
  if (child == nullptr || child->parent() != nullptr) {
    qWarning("Child has already a parent");
    return false;
  }

  if (child->thread() != this->thread()) {
    if (QThread::currentThread() != child->thread()) {
      qWarning("The thread owning the child must call 'addChild'.");
      return false;
    }
    child->moveToThread(this->thread());
  }

  return QMetaObject::invokeMethod(this, [this, child] {
    child->setParent(this);
  });
}

bool Connection::removeChild(QObject *child) {
  if (child == nullptr || child->thread() != this->thread()) {
    return false;
  }

  return QMetaObject::invokeMethod(this, [child] {
    child->deleteLater();
  });
}

}