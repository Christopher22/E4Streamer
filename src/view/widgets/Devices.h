//
// Created by christopher on 27.02.2020.
//

#ifndef E4STREAMER_SRC_VIEW_WIDGETS_DEVICES_H_
#define E4STREAMER_SRC_VIEW_WIDGETS_DEVICES_H_

#include <QListWidget>

namespace e4streamer {
namespace model {
class Connection;
class Device;
}

namespace view::widgets {
class Devices : public QListWidget {
 Q_OBJECT

 public:
  explicit Devices(bool connect_manually = true, QWidget *parent = nullptr);
  bool setConnection(model::Connection *connection);
  void setConnectManually(bool connect_manually);

  bool updateDevices();
  [[nodiscard]] model::Device *selectedDevice() const;

 signals:
  void deviceSelected(model::Device *device);

 protected:
  void setSelectedDevice(model::Device *device);

 private:
  model::Connection *connection_;
  model::Device *selected_device_;
  bool connect_manually_;

  void _connectDevice(model::Device *device);
  void _selectDevice(QListWidgetItem *item);
  void _onSelectedDevice(model::Device *device);
  void _handleUpdateFailure(const QString &error);
  void _handleConnectionFailure(const QString &error);
  void _handleDisconnectionFailure(const QString &error);
  void _updateDevices(const QVector<model::Device *> &devices);
  void _clearDevices();
};
}
}

#endif //E4STREAMER_SRC_VIEW_WIDGETS_DEVICES_H_
