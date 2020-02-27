//
// Created by christopher on 26.02.2020.
//

#include "Server.h"
#include "Connection.h"

#include <QProcess>
#include <QThread>
#include <QHostAddress>
#include <QFile>

namespace e4streamer::model {

Server::Server(QString server_path, QString api_key, quint16 port, QObject *parent)
	: QObject(parent),
	  server_path_(std::move(server_path)),
	  api_key_(std::move(api_key)),
	  port_(port),
	  connection_(nullptr),
	  background_worker_(nullptr),
	  background_server_(nullptr),
	  state_(State::NotConnected) {}

Server::~Server() {
  this->_cleanUp();
}

bool Server::start() {
  if (!QFile::exists(server_path_) || api_key_.isEmpty()) {
	return false;
  }

  // Create the background server process
  background_server_ = new QProcess(this);
  background_server_->setProcessChannelMode(QProcess::MergedChannels);

  // Handle the successful start of the server
  QObject::connect(background_server_, &QProcess::readyReadStandardOutput, [this]() {
	// Read the status message and check for an error...
	QString message = QString::fromUtf8(background_server_->readAll()).trimmed();

	// Ignore runtime writing
	if (message.isEmpty() || state_ != State::ServerStarting) {
	  return;
	}

	if (message.contains("errors", Qt::CaseInsensitive)) {
	  this->_cleanUp();

	  // Prettify Empathica error code. Example: "following errors were discovered:\r\n\r\n- invalid port number specified"
	  const int error_msg_starts = message.lastIndexOf('-');
	  if (error_msg_starts != -1 && message.size() > error_msg_starts + 2) {
		message = message.mid(error_msg_starts + 1).trimmed();
	  }

	  emit this->connectionFailed(tr("The Empathica server has thrown an error: %1").arg(message));
	  return;
	}

	// ... otherwise create the background worker and the corresponding connection ...
	qRegisterMetaType<QHostAddress>("QHostAddress");
	qRegisterMetaType<QIODevice::OpenMode>("QIODevice::OpenMode");

	connection_ = new Connection(nullptr);
	QObject::connect(this,
					 &Server::connecting,
					 connection_,
					 qOverload<const QHostAddress &, quint16, QIODevice::OpenMode>(&Connection::connectToHost));
	QObject::connect(connection_, &Connection::connected, [this] {
	  state_ = State::Connected;
	  emit this->connected(connection_);
	});
	QObject::connect(connection_,
					 qOverload<QAbstractSocket::SocketError>(&Connection::error),
					 [this](QAbstractSocket::SocketError socketError) {
					   const QString error_msg = connection_->errorString();
					   this->_cleanUp();
					   emit this->connectionFailed(tr("Unable to open the TCP pipeline: %1").arg(error_msg));
					 });

	// ... and start them.
	background_worker_ = new QThread(this);
	QObject::connect(background_worker_, &QThread::destroyed, connection_, &QObject::deleteLater);
	connection_->moveToThread(background_worker_);

	state_ = State::Connecting;
	background_worker_->start();
	emit this->connecting(QHostAddress::LocalHost, port_, QIODevice::ReadWrite);
  });

  // Handle failure during start up
  QObject::connect(background_server_, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
	const QString error_msg = background_server_->errorString();
	this->_cleanUp();
	emit this->connectionFailed(tr("The underlying Empathica server process failed. %1").arg(error_msg));
  });

  // Start the background server
  state_ = State::ServerStarting;
  background_server_->start(server_path_, {api_key_, "127.0.0.1", QString::number(port_, 10)});
  return true;
}

void Server::_cleanUp() {
  state_ = State::NotConnected;

  // The background worker takes care of the delete
  connection_ = nullptr;

  if (background_worker_ != nullptr) {
	background_worker_->quit();
	background_worker_->wait();
	background_worker_->deleteLater();
	background_worker_ = nullptr;
  }

  if (background_server_ != nullptr) {
	if (background_server_->state() == QProcess::Running) {
	  background_server_->kill();
	}
	background_server_->deleteLater();
	background_server_ = nullptr;
  }
}

}

