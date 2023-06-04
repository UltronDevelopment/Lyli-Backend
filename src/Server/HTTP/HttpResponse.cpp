// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpResponse.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    04 June 2023
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.
//
// (c) 2023 UPN
//

#include <Server/HTTP/HttpResponse.hpp>

#include <sstream>

namespace Lyli::Server::HTTP {
HttpResponse::HttpResponse() = default;
HttpResponse::~HttpResponse() = default;

void HttpResponse::setCode(ResponseCode code) { this->code = code; }

std::string HttpResponse::toString() const {
  std::stringstream buffer;

  /* version and status code */
  buffer << "HTTP/1.1" << static_cast<std::uint16_t>(this->code)
         << HttpResponse::codeToString(this->code) << "\r\n";

  /* header */
  for (const auto &[name, value] : this->header) {
    buffer << name << ": " << value << "\r\n";
  }

  /* data */
  buffer << "\r\n" << this->data;

  return buffer.str();
}

std::string_view HttpResponse::codeToString(ResponseCode code) {
  switch (code) {
  case ResponseCode::NOT_FOUND:
    return "NOT FOUND";
  case ResponseCode::OK:
    return "OK";
  default:
    return "NONE";
  }
}
} // namespace Lyli::Server::HTTP