//
// Created by Christopher Gundler on 01.03.2020.
//

#ifndef E4STREAMER_SRC_MODEL_DISCONNECTABLE_H_
#define E4STREAMER_SRC_MODEL_DISCONNECTABLE_H_

#include <QObject>

namespace e4streamer::model {

/*
 * The connection stays open until all its children are gone.
 * This marker interface is used to indicate that a class knows about this fact.
 */
class Disconnectable : public QObject {
 Q_OBJECT

 public:
  inline explicit Disconnectable(QObject *parent = nullptr) : QObject(parent) {}
  virtual void handleDisconnect() = 0;
};
}

#endif //E4STREAMER_SRC_MODEL_DISCONNECTABLE_H_
