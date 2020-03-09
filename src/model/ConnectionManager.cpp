//
// Created by Christopher Gundler on 01.03.2020.
//

#include "ConnectionManager.h"
#include "Connection.h"

namespace e4streamer::model {

ConnectionManager::ConnectionManager(const QHostAddress &address, quint16 port, QObject *parent)
	: QThread(parent), connection_(nullptr), address_(address), port_(port) {

}

void ConnectionManager::run() {
  qDebug() << "Creating connection...";
  connection_ = new Connection(nullptr);
  QObject::connect(connection_, &QObject::destroyed, [this] {
	connection_ = nullptr;
	this->quit();
  });

  connection_->connectToHost(address_, port_);
  if (connection_->waitForConnected(3000)) {
	emit this->connected(connection_);

	qDebug() << "Connection created. Running message queue of manager.";
	this->exec();

	qDebug() << "Message queue of manager ended.";
	if (connection_ != nullptr) {
	  qWarning() << "Connection did not deleted itself. Manager will handle that.";
	  connection_->deleteLater();
	  connection_ = nullptr;
	}
  } else {
	qDebug() << "Connection failed: " << connection_->errorString();
	emit this->connectionFailed(connection_->errorString());
	connection_->deleteLater();
	connection_ = nullptr;
  }
}

void ConnectionManager::shutdown(unsigned long timeout) {
  if (connection_ != nullptr) {
	Q_ASSERT(QThread::currentThread() != connection_->thread());
	QMetaObject::invokeMethod(connection_, [&] {
	  connection_->disconnectFromEmpathica();
	});

	if (!this->wait(timeout)) {
	  qDebug() << "Still pending children. Killing them and the connection now.";
	  QMetaObject::invokeMethod(connection_, [&] {
		connection_->kill();
	  });
	}

	// Wait until thread exited
	this->wait();
	connection_ = nullptr;
  }
}

}