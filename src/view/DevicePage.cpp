//
// Created by christopher on 27.02.2020.
//

#include "DevicePage.h"
#include "Wizard.h"
#include "widgets/Devices.h"
#include "../model/Connection.h"
#include "../model/Device.h"

#include <QKeySequence>
#include <QVBoxLayout>

namespace e4streamer::view {

DevicePage::DevicePage(QWidget *parent) : QWizardPage(parent), devices_(new widgets::Devices(this)) {
  this->setTitle(tr("Step 2: Connect to a E4"));
  this->setSubTitle(tr(
	  "Please select a device by double-clicking on its entry. Use %1 to update the devices.")
						.arg(QKeySequence(QKeySequence::Refresh).toString(QKeySequence::NativeText)));
  this->setCommitPage(true);
  auto layout = new QVBoxLayout(this);
  layout->addWidget(devices_);
  this->setLayout(layout);

  QObject::connect(devices_, &widgets::Devices::deviceSelected, [this](model::Device *device) {
	emit this->completeChanged();
  });
}

void DevicePage::initializePage() {
  QWizardPage::initializePage();

  auto *wizard = qobject_cast<Wizard *>(this->wizard());
  if (wizard == nullptr || wizard->connection() == nullptr || !this->devices_->setConnection(wizard->connection())) {
	qWarning("Unable to acquire connection!");
  }

  devices_->updateDevices();
}

bool DevicePage::validatePage() {
  return QWizardPage::validatePage();
}

bool DevicePage::isComplete() const {
  return QWizardPage::isComplete() && devices_->selectedDevice() != nullptr;
}

}