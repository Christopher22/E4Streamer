//
// Created by Christopher Gundler on 29.02.2020.
//

#ifndef E4STREAMER_SRC_MODEL_SAMPLE_H_
#define E4STREAMER_SRC_MODEL_SAMPLE_H_

#include <QString>
#include <QVector>

#include <string_view>

namespace e4streamer::model {

class Sample : public QVector<float> {
 public:
  enum class Type {
    Acceleration,
    BloodVolumePulse,
    GalvanicSkinResponse,
    Heartbeat,
    SkinTemperature,
    Battery,
    Tags,
    SampleError
  };

  static Sample parse(const QString &sample);
  inline explicit operator bool() const noexcept {
    return type_ != Type::SampleError;
  }

  [[nodiscard]] inline Type type() const {
    return type_;
  }

  [[nodiscard]] inline double timestamp() const {
    return timestamp_;
  }

  [[nodiscard]] constexpr static const char *id(Type type) {
    switch (type) {
      case Type::Acceleration: return "acc";
      case Type::BloodVolumePulse: return "bvp";
      case Type::GalvanicSkinResponse: return "gsr";
      case Type::SkinTemperature: return "tmp";
      case Type::Heartbeat: return "ibi";
      case Type::Battery: return "bat";
      case Type::Tags: return "tag";
      case Type::SampleError: return "";
    }
  }

  [[nodiscard]] constexpr static int numChannels(Type type) {
    switch (type) {
      case Type::Acceleration: return 3;
      case Type::BloodVolumePulse:
      case Type::GalvanicSkinResponse:
      case Type::SkinTemperature:
      case Type::Heartbeat:
      case Type::Battery: return 1;
      case Type::Tags:
      case Type::SampleError: return 0;
    }
  }

  [[nodiscard]] constexpr static Type type(const char *id) {
    for (auto type: {Type::Acceleration, Type::BloodVolumePulse, Type::GalvanicSkinResponse, Type::SkinTemperature,
                     Type::Heartbeat, Type::Battery, Type::Tags}) {
      if (std::string_view(Sample::id(type)) == id) {
        return type;
      }
    }
    return Type::SampleError;
  }

 protected:
  inline Sample() noexcept: QVector<float>(), type_(Type::SampleError), timestamp_(0.0f) {}
  inline Sample(Type type, double timestamp) noexcept
      : QVector<float>(Sample::numChannels(type)), type_(type), timestamp_(timestamp) {}

 private:
  Type type_;
  double timestamp_;
};
}

#endif //E4STREAMER_SRC_MODEL_SAMPLE_H_
