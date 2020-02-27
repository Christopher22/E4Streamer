//
// Created by christopher on 26.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_CONNECTION_H_
#define E4STREAMER_SRC_MODEL_CONNECTION_H_

#include <QTcpSocket>
#include <QVector>
#include <functional>

namespace e4streamer::model {
class Command;

class Connection : public QTcpSocket {
 Q_OBJECT

 public:
  template<typename T>
  class QueuedCommand {
   public:
	inline QueuedCommand(T *command, Connection *connection) : command_(command), connection_(connection) {
	  command->moveToThread(connection->thread());
	  command->setParent(connection);
	}

	~QueuedCommand() {
	  connection_->writeLine(command_->rawCommand());
	}

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

  template<typename T, typename... Args>
  QueuedCommand<T> send(Args &&... args) {
	auto *command = new T(std::forward<Args>(args)...);
	commands_.push_back(command);
	return QueuedCommand(command, this);
  }

  void writeLine(const QString &line);

 private:
  void _processReceivedData();

  QVector<Command *> commands_;
};
}

#endif //E4STREAMER_SRC_MODEL_CONNECTION_H_
