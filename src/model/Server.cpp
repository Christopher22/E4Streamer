//
// Created by christopher on 26.02.2020.
//

#include "Server.h"
#include "Connection.h"

#include <QProcess>
#include <QThread>
#include <QHostAddress>
#include <QFile>
#include <QDebug>

namespace e4streamer::model {

Server::Server(QString server_path, QString api_key, quint16 port, QObject *parent)
	: QObject(parent),
	  server_path_(std::move(server_path)),
	  api_key_(std::move(api_key)),
	  port_(port),
	  connection_(nullptr),
	  background_worker_(nullptr),
	  background_server_(nullptr),
	  state_(State::NotConnected) {

  // Enforce cleaning in case of error
  QObject::connect(this, &Server::connectionFailed, [&](const QString &) {
	this->_cleanUp();
  });
}

Server::~Server() {
  this->_cleanUp();
}

bool Server::start() {
  if (!this->isReady()) {
	return false;
  }

  // Create the background server process
  qDebug("Creating server...");
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

	qDebug() << "Successfully connected to Empathica server. Got message: " << message;

	if (message.contains("errors", Qt::CaseInsensitive)) {
	  // Prettify Empathica error code. Example: "following errors were discovered:\r\n\r\n- invalid port number specified"
	  const int error_msg_starts = message.lastIndexOf('-');
	  if (error_msg_starts != -1 && message.size() > error_msg_starts + 2) {
		message = message.mid(error_msg_starts + 1).trimmed();
	  }

	  qDebug() << "Empathica responded failure: " << message << ". Calling 'connectionFailed'.";
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
	  qDebug() << "Connection established. Firing 'connected'.";
	  state_ = State::Connected;
	  emit this->connected(connection_);
	});
	QObject::connect(connection_,
					 qOverload<QAbstractSocket::SocketError>(&Connection::error),
					 [this](QAbstractSocket::SocketError socketError) {
					   qDebug() << "Connection failed. Firing 'connectionFailed'.";
					   emit this
						 ->connectionFailed(tr("Unable to open the TCP pipeline: %1").arg(connection_->errorString()));
					 });

	// ... and start them.
	qDebug() << "Starting worker...";
	background_worker_ = new QThread(this);
	QObject::connect(background_worker_, &QThread::finished, connection_, &QObject::deleteLater);
	connection_->moveToThread(background_worker_);

	state_ = State::Connecting;
	background_worker_->start();
	emit this->connecting(QHostAddress::LocalHost, port_, QIODevice::ReadWrite);
	qDebug() << "Worker started asynchronously.";
  });

  // Handle failure during start up
  QObject::connect(background_server_, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
	// Fail only once before clean-up.
	if (state_ == State::ServerStarting) {
	  emit this->connectionFailed(tr("The underlying Empathica server process failed. %1")
									  .arg(background_server_->errorString()));
	}
  });

  // Handle failure during start up
  connect(background_server_,
		  QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		  [this](int exitCode, QProcess::ExitStatus exitStatus) {
			qDebug() << "Server finished with exit code " << exitCode;
			if (state_ == State::ServerStarting) {
			  emit this
				  ->connectionFailed(tr("The underlying Empathica server process ended unexpectedly with exit code %1.")
										 .arg(exitCode));
			}
		  });

  // Start the background server
  qDebug("Starting server...");
  state_ = State::ServerStarting;
  background_server_->start(server_path_, {api_key_, "127.0.0.1", QString::number(port_, 10)});
  return true;
}

void Server::_cleanUp() {
  // Ensure cleaning only once
  if (state_ == State::NotConnected) {
	return;
  }

  state_ = State::NotConnected;

  // The background worker takes care of the delete
  connection_ = nullptr;

  if (background_worker_ != nullptr) {
	background_worker_->quit();
	if (!background_worker_->wait(1000)) {
	  qWarning("Unable to wait for the background worker!");
	}
	background_worker_->deleteLater();
	background_worker_ = nullptr;
  }

  if (background_server_ != nullptr) {
	if (background_server_->state() != QProcess::NotRunning) {
	  background_server_->kill();
	}
	background_server_->deleteLater();
	background_server_ = nullptr;
  }
}

bool Server::isReady() const noexcept {
  return QFile::exists(server_path_) && !api_key_.isEmpty();
}

bool Server::setServerPath(const QString &server_path) {
  return this->_set_value(server_path, [this](const QString &server_path) {
	server_path_ = server_path;
  });
}

bool Server::setApiKey(const QString &api_key) {
  return this->_set_value(api_key, [this](const QString &api_key) {
	api_key_ = api_key;
  });
}

bool Server::setPort(quint16 port) {
  return this->_set_value(port, [this](const quint16 &port) {
	port_ = port;
  });
}

}

