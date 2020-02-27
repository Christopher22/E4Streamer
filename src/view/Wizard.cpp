//
// Created by christopher on 27.02.2020.
//

#include "Wizard.h"
#include "ServerPage.h"

namespace e4streamer::view {
Wizard::Wizard(QWidget *widget) : QWizard(widget) {
  this->setOption(QWizard::HaveCustomButton1, true);
  this->setOption(QWizard::HaveHelpButton, false);

  this->addPage(new ServerPage(this));
}
}

