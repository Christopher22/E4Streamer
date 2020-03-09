//
// Created by Christopher Gundler on 29.02.2020.
//

#include "Sample.h"

#include <QRegExp>
#include <QStringList>
#include <QLocale>

namespace e4streamer::model {

Sample Sample::parse(const QString &raw_sample) {
  // The streaming server formats floating point numbers in the system locale.
  static QLocale locale = QLocale::system();

  QStringList parts = raw_sample.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  if (parts.size() < 2) {
	return Sample();
  }

  bool float_ok;
  Sample sample(
	  Sample::responseType(parts[0].toLatin1()),
	  locale.toDouble(parts[1], &float_ok)
  );

  // Fill the channels, if the given number of arguments is appropriated.
  if (sample.size() == parts.size() - 2) {
	for (int i = 2, size = parts.size(); i < size && float_ok; ++i) {
	  sample[i - 2] = locale.toFloat(parts[i], &float_ok);
	}
  } else {
	return Sample();
  }

  return float_ok ? sample : Sample();
}

}