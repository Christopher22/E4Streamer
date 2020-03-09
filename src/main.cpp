#include <QApplication>
#include <QPointer>
#include "view/Wizard.h"

int main(int argc, char **argv) {
  QApplication application(argc, argv);
  QApplication::setApplicationName("E4Streamer");
  QApplication::setOrganizationName("Christopher Gundler");
  QApplication::setApplicationVersion("0.2.0.0");

  auto *wizard = new e4streamer::view::Wizard();
  QObject::connect(wizard, &e4streamer::view::Wizard::finished, [&] {
    qDebug("Recording done. Deleting wizard...");
    wizard->deleteLater();
  });
  wizard->show();
  return QApplication::exec();
}
