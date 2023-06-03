// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpRequest.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    28 May 2023
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

#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <string_view>

namespace Lyli::Server::HTTP {
enum class RequestType : std::size_t { NONE, GET, POST, PATCH, DELETE };

class HttpRequest {
public:
  explicit HttpRequest(std::string_view data);
  ~HttpRequest();

  RequestType getRequestType() const;
  std::string_view getPath() const;
  std::string_view getHeaderValue(const std::string &key) const;
  std::string_view getData() const;
  bool isValid() const;

  static std::string_view requestTypeToString(RequestType type);

private:
  bool parse(std::string_view data);
  bool firstParse(std::string_view data);
  std::pair<bool, const char *> headerParse(std::string_view data);
  bool dataParse(std::string_view data);

  RequestType checkRequestType(std::string_view data) const;

  RequestType request_type;
  std::string path;

  std::map<std::string, std::string, std::less<>> header;
  std::string data;

  bool valid;
};
} // namespace Lyli::Server::HTTP