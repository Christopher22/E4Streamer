//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Sample.h"

#include <QRegExp>
#include <QStringList>

namespace e4streamer::model {

Sample Sample::parse(const QString &raw_sample) {
  QStringList parts = raw_sample.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  if (parts.size() < 2) {
    return Sample();
  }

  bool float_ok;
  Sample sample(
      Sample::type(parts[0].toLatin1()),
      parts[1].toDouble(&float_ok)
  );

  // Fill the channels, if the given number of arguments is appropriated.
  if (sample.size() == parts.size() - 2) {
    for (int i = 2, size = parts.size(); i < size && float_ok; ++i) {
      sample[i - 2] = parts[i].toFloat(&float_ok);
    }
  } else {
    return Sample();
  }

  return float_ok ? sample : Sample();
}

}