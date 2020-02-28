//
// Created by christopher on 28.02.2020.
//

#ifndef E4STREAMER_TESTS_COMMANDTESTS_H_
#define E4STREAMER_TESTS_COMMANDTESTS_H_

#include <QObject>

class CommandTests : public QObject {
 Q_OBJECT

 public:
  CommandTests();

 private slots:
  void testParsing();
};

#endif //E4STREAMER_TESTS_COMMANDTESTS_H_
