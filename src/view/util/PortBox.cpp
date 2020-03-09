//
// Created by christopher on 09.03.2020.
//

#include "PortBox.h"

#include <limits>

namespace e4streamer::view::util {

PortBox::PortBox(QWidget *parent) : QSpinBox(parent) {
  this->setRange(1024, std::numeric_limits<quint16>::max());
  this->setValue(28000);
}

}