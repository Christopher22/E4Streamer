//
// Created by christopher on 27.02.2020.
//

#include "Wizard.h"
#include "ServerPage.h"
#include "RemoteConnectionPage.h"
#include "DevicePage.h"
#include "RecordingPage.h"

namespace e4streamer::view {
Wizard::Wizard(QWidget *widget)
	: QWizard(widget, Qt::WindowFlags(Qt::MSWindowsFixedSizeDialogHint)),
	  connection_page_(new RemoteConnectionPage(this)),
	  device_page_(new DevicePage(this)),
	  recording_page_(new RecordingPage(this)) {
  this->setOption(QWizard::HaveCustomButton1, true);
  this->setOption(QWizard::HaveHelpButton, false);

  this->addPage(connection_page_);
  this->addPage(device_page_);
  this->addPage(recording_page_);
}

model::Connection *Wizard::connection() {
  return connection_page_->connection();
}
}

