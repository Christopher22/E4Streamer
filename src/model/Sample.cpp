//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Sample.h"

#include <QRegExp>
#include <QStringList>

namespace e4streamer::model {

Sample Sample::parse(const QString &raw_sample) {
  QStringList parts = raw_sample.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  if (parts.size() != 3) {
    return Sample();
  }

  bool timestamp_ok, data_ok;
  Sample sample(
      Sample::type(parts[0].toLatin1()),
      parts[1].toFloat(&timestamp_ok),
      parts[2].toFloat(&data_ok)
  );
  return timestamp_ok && data_ok ? sample : Sample();
}

}