//
// Created by christopher on 26.02.2020.
//

#include "Connection.h"
#include "Command.h"
#include "Response.h"
#include "Disconnectable.h"

#include <QtDebug>
#include <QThread>
#include <QTimer>
#include <QChildEvent>

namespace e4streamer::model {

Connection::Connection(QObject *parent) : QTcpSocket(parent), is_shutting_down_(false) {
  QObject::connect(this, &QTcpSocket::readyRead, this, &Connection::_processReceivedData);
}

Connection::~Connection() {
  this->disconnectFromEmpathica();
}

void Connection::_writeLine(const QString &line) {
  this->write(line.toUtf8());
  this->write("\r\n");
  qDebug() << "Wrote" << line << "to server.";
}

void Connection::_processReceivedData() {
  while (this->canReadLine()) {
    const QString received_string = QString::fromUtf8(this->readLine());

    if (received_string.startsWith('R')) {
      qDebug() << "Received response: " << received_string;

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
      qDebug() << "Received sample: " << received_string;

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

  qDebug("Sending warning that connection will close soon...");
  emit this->disconnecting();

  // Disconnect if no children are there, otherwise wait for them.
  this->_clear();
}

void Connection::registerCommand(Command *command) {
  Q_ASSERT(command->thread() == this->thread());
  Q_ASSERT(command->parent() == nullptr);

  command->setParent(this);
  commands_.push_back(command);
  this->_writeLine(command->rawCommand());
}

bool Connection::addChild(Disconnectable *child) {
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

bool Connection::removeChild(Disconnectable *child) {
  if (child == nullptr || child->thread() != this->thread()) {
    return false;
  }

  return QMetaObject::invokeMethod(this, [child] {
    child->deleteLater();
  });
}

void Connection::childEvent(QChildEvent *event) {
  QObject::childEvent(event);

  // Check if all children are removed now
  if (is_shutting_down_ && event->removed()) {
    this->_clear();
  }
}

bool Connection::_clear() {
  auto disconnectables = this->findChildren<Disconnectable *>(QString(), Qt::FindDirectChildrenOnly);
  if (disconnectables.empty()) {
    is_shutting_down_ = true;

    // Warn about objects potentially not ready for disconnection.
    for (auto *other_child: this->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly)) {
      qWarning() << "Connection still has child" << other_child->objectName() << ", which will be deleted NOW!";
    }

    this->kill();
    return true;
  } else if (!is_shutting_down_) {
    qDebug("Calling 'handleDisconnect' on all disconnectables...");

    // Let the children handle the disconnection in future
    for (auto *disconnectable: disconnectables) {
      disconnectable->handleDisconnect();
    }
    is_shutting_down_ = true;
  }
  return false;
}

void Connection::kill() {
  // Close socket
  qDebug("Closing connection to Empathica server...");
  this->disconnectFromHost();
  if (this->state() == ConnectedState && !this->waitForDisconnected(1000)) {
    qWarning() << "Unable to disconnect from socket: " << this->errorString();
  } else {
    qDebug("Disconnected from Empathica server.");
  }

  // Delete connection
  this->deleteLater();
}

}