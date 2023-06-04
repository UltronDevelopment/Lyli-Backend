// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpObject.hpp
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

#pragma once

#include <map>
#include <string>
#include <string_view>

namespace Lyli::Server::HTTP {

enum class RequestType : std::size_t { NONE, GET, POST, PATCH, DELETE };

class HttpObject {
public:
  HttpObject();
  ~HttpObject();

  std::string_view getHeaderValue(const std::string &key) const;
  std::string_view getData() const;
  bool isValid() const;

  void setHeaderValue(const std::string &key, std::string_view value);
  void setData(std::string_view data);

  static std::string_view requestTypeToString(RequestType type);

protected:
  std::map<std::string, std::string, std::less<>> header;
  std::string data;

  bool valid;
};
} // namespace Lyli::Server::HTTP