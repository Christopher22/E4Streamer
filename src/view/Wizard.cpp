//
// Created by christopher on 27.02.2020.
//

#include "Wizard.h"
#include "ServerPage.h"
#include "DevicePage.h"

namespace e4streamer::view {
Wizard::Wizard(QWidget *widget)
	: QWizard(widget), server_page_(new ServerPage(this)), device_page_(new DevicePage(this)) {
  this->setOption(QWizard::HaveCustomButton1, true);
  this->setOption(QWizard::HaveHelpButton, false);

  this->addPage(server_page_);
  this->addPage(device_page_);
}

model::Connection *Wizard::connection() {
  return server_page_->connection();
}

model::Device *Wizard::device() {
  return nullptr;
}
}

