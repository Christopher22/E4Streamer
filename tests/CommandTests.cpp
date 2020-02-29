//
// Created by christopher on 28.02.2020.
//

#include "ConnectionTests.h"
#include "../src/model/Response.h"
#include "../src/model/Sample.h"
#include "../src/model/commands/DiscoverDevices.h"

#include <QtTest>

ConnectionTests::ConnectionTests() : QObject(nullptr) {

}

void ConnectionTests::testResponseParsing() {
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

void ConnectionTests::testSampleParsing() {
  auto sample = e4streamer::model::Sample::parse("acc 123.456 1.9 2 3");
  QCOMPARE(sample.type(), e4streamer::model::Sample::Type::Acceleration);
  QCOMPARE(sample.timestamp(), 123.456);
  QCOMPARE(sample.size(), 3);
  QCOMPARE(sample[0], 1.9f);
  QCOMPARE(sample[1], 2.0f);
  QCOMPARE(sample[2], 3.0f);
}