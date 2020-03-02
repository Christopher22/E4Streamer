//
// Created by Christopher Gundler on 01.03.2020.
//

#include "ConnectionManager.h"
#include "Connection.h"
#include <QPointer>

namespace e4streamer::model {

ConnectionManager::ConnectionManager(QObject *parent) : QThread(parent), connection_(nullptr) {

}

void ConnectionManager::run() {
  qDebug() << "Creating connection...";
  connection_ = new Connection(nullptr);
  QObject::connect(connection_, &QObject::destroyed, [this] {
    connection_ = nullptr;
    this->quit();
  });

  emit this->connectionCreated(connection_);

  qDebug() << "Connection created. Running message queue of manager.";
  this->exec();

  qDebug() << "Message queue of manager ended.";
  if (connection_ != nullptr) {
    qWarning() << "Connection did not deleted itself. Manager will handle that.";
    connection_->deleteLater();
  }
}

void ConnectionManager::shutdown(unsigned long timeout) {
  if (connection_ != nullptr) {
    Q_ASSERT(QThread::currentThread() != connection_->thread());
    QMetaObject::invokeMethod(this, [&] {
      connection_->disconnectFromEmpathica();
    });

    if (!this->wait(timeout)) {
      qDebug() << "Still pending children. Killing them and the connection now.";
      QMetaObject::invokeMethod(this, [&] {
        connection_->kill();
      });
    }
  }
}

}