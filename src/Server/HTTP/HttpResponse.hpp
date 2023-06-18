// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpResponse.hpp
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

#include <Server/HTTP/HttpObject.hpp>

#include <cstdint>
#include <memory>
#include <string_view>

namespace Lyli::Server::HTTP {

enum class ResponseCode : std::uint16_t {
  OK = 200,
  NO_CONTENT = 204,
  BAD_REQUEST = 400,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405
};

class HttpResponse : public HttpObject {
public:
  HttpResponse();
  ~HttpResponse();

  void setCode(ResponseCode code);

  std::string toString() const;

  static std::string_view codeToString(ResponseCode code);

  /* allocate an empty HttpResponse */
  static std::shared_ptr<HttpResponse> create();

private:
  ResponseCode code;
};
} // namespace Lyli::Server::HTTP