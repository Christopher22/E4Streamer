//
// Created by christopher on 28.02.2020.
//

#include "CommandTests.h"
#include "../src/model/Response.h"
#include "../src/model/commands/DiscoverDevices.h"

#include <QtTest>

CommandTests::CommandTests() : QObject(nullptr) {

}

void CommandTests::testParsing() {
  auto response = e4streamer::model::Response::parse(nullptr,
													 "R device_discover_list 2 | 9ff167 Empatica_E4 available | 740163 Empatica_E4 available");

  QCOMPARE(response->size(), 9);
  QCOMPARE(response->command(), QString("device_discover_list"));
  QCOMPARE(response->at(0), QString("2"));
  QCOMPARE(response->at(8), QString("available"));

  auto *command = new e4streamer::model::commands::DiscoverDevices(nullptr);
  QVERIFY(command->isSuitable(response.get()));
  command->deleteLater();
}
