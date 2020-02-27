//
// Created by christopher on 26.02.2020.
//

#include "Response.h"

namespace e4streamer::model {

Response::Response(QString response_target, QStringList &&arguments)
	: QStringList(arguments), command_(std::move(response_target)), is_accepted_(false) {

}

std::unique_ptr<Response> Response::parse(const QString &raw_response) {
  QStringList parts = raw_response.split(QRegExp("\\s+"), QString::SkipEmptyParts);
  if (parts.length() < 2 || parts[0] != "R") {
	return nullptr;
  }

  const QString command = parts[1];
  parts.pop_front();
  parts.pop_front();
  return std::unique_ptr<Response>(new Response(command, std::move(parts)));
}

}