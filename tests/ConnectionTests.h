//
// Created by christopher on 28.02.2020.
//

#ifndef E4STREAMER_TESTS_COMMANDTESTS_H_
#define E4STREAMER_TESTS_COMMANDTESTS_H_

#include <QObject>

class ConnectionTests : public QObject {
 Q_OBJECT

 public:
  ConnectionTests();

 private slots:
  void testResponseParsing();
  void testSampleParsing();
};

#endif //E4STREAMER_TESTS_COMMANDTESTS_H_
