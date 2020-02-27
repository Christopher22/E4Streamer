#include <QApplication>

#include "view/Wizard.h"

int main(int argc, char **argv) {
  QApplication application(argc, argv);
  QApplication::setApplicationName("E4Streamer");
  QApplication::setOrganizationName("Christopher Gundler");
  QApplication::setApplicationVersion("0.1");

  auto *server_config = new e4streamer::view::Wizard();
  server_config->show();

  return QApplication::exec();
}
