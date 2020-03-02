//
// Created by Christopher Gundler on 29.02.2020.
//

#include "RecordingPage.h"
#include "widgets/StreamWidget.h"
#include "Wizard.h"

#include <QVBoxLayout>

namespace e4streamer::view {

RecordingPage::RecordingPage(QWidget *parent) : QWizardPage(parent) {
  this->setSubTitle(tr(
      "The recording is ready und the Lab Streaming Layer streams created. You may now register them in the recording software. Simply check the stream of interest below to subscribe to them and get the data."));
  auto *layout = new QVBoxLayout(this);
  for (auto type: model::Sample::AVAILABLE_TYPES) {
    layout->addWidget(new widgets::StreamWidget(type, this));
  }
  this->setLayout(layout);
}

void RecordingPage::initializePage() {
  QWizardPage::initializePage();

  auto *wizard = qobject_cast<Wizard *>(this->wizard());
  auto *connection = wizard != nullptr ? wizard->connection() : nullptr;
  if (connection != nullptr) {
    for (auto *child: this->findChildren<widgets::StreamWidget *>(QString(), Qt::FindDirectChildrenOnly)) {
      child->setConnection(connection);
    }
  }
}

}

