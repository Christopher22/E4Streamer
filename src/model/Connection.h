//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_CONNECTION_H_
#define E4STREAMER_SRC_MODEL_CONNECTION_H_

#include "Command.h"
#include "Sample.h"

#include <QTcpSocket>
#include <QVector>

namespace e4streamer::model {
class Connection : public QTcpSocket {
 Q_OBJECT

 public:
  template<typename T>
  class QueuedCommand {
   public:
	inline QueuedCommand(T *command, Connection *connection) : command_(command), connection_(connection) {
	  static_assert(std::is_base_of<Command, T>::value, "Type T is not a command");
	}

	~QueuedCommand() {
	  // Transfer the command to the other thread
	  command_->moveToThread(connection_->thread());
	  const bool
		  result =
		  QMetaObject::invokeMethod(connection_, "registerCommand", Qt::QueuedConnection, Q_ARG(Command*, command_));
	  Q_ASSERT(result);
	}

	QueuedCommand(const QueuedCommand &) = delete;
	QueuedCommand &operator=(QueuedCommand const &) = delete;

	inline T *command() noexcept {
	  return command_;
	}

	inline explicit operator T *() noexcept {
	  return command_;
	}

   private:
	T *command_;
	Connection *connection_;
  };

  explicit Connection(QObject *parent = nullptr);
  ~Connection() override;
  void disconnectFromEmpathica();
  void kill();

  bool addChild(Disconnectable *child);
  bool removeChild(Disconnectable *child);
  bool processData();
  Q_INVOKABLE void registerCommand(Command *command);

  template<typename T, typename... Args>
  QueuedCommand<T> send(Args &&... args) {
	// This is called from an external thread
	auto *command = new T(std::forward<Args>(args)...);
	return QueuedCommand(command, this);
  }

 signals:
  void disconnecting();
  void sample(const Sample &sample);

 protected:
  void childEvent(QChildEvent *event) override;

 private:
  enum class ShutdownState {
	NoShutdown,
	WaitingForChildren,
	Killing
  };

  bool _clear();
  void _writeLine(const QString &line);
  void _handleResponse(const QString &response);

  QByteArray buffer_;
  QVector<Command *> commands_;
  ShutdownState shutdown_state_;
};
}

#endif //E4STREAMER_SRC_MODEL_CONNECTION_H_
