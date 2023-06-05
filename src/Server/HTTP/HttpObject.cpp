// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpObject.cpp
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

#include <Server/HTTP/HttpObject.hpp>

namespace Lyli::Server::HTTP {
HttpObject::HttpObject() = default;
HttpObject::~HttpObject() = default;

std::string_view HttpObject::getHeaderValue(const std::string &key) const {
  if (this->header.contains(key))
    return this->header.at(key);

  return "";
}
std::string_view HttpObject::getData() const { return this->data; }
bool HttpObject::isValid() const { return this->valid; }

void HttpObject::setHeaderValue(const std::string &key,
                                std::string_view value) {
  this->header[key] = value;
}

void HttpObject::setData(std::string_view data) { this->data = data; }

std::string_view HttpObject::requestTypeToString(RequestType type) {
  switch (type) {
  case RequestType::GET:
    return "GET";
  case RequestType::POST:
    return "POST";
  case RequestType::PATCH:
    return "PATCH";
  case RequestType::DELETE:
    return "DELETE";
  default:
    return "";
  }
}

} // namespace Lyli::Server::HTTP