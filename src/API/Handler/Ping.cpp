// This file is part of the Lyli Backend
// File:    API/Handler/Ping.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    05 June 2023
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

#include <API/Handler/Ping.hpp>
#include <Server/HTTP/HttpResponse.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>
#include <memory>

namespace Lyli::API::Handler {
std::shared_ptr<Server::HTTP::HttpResponse> ping([
    [maybe_unused]] const std::shared_ptr<Server::HTTP::HttpRequest> &request) {

  auto resp = Server::HTTP::HttpResponse::create();
  if (resp == nullptr)
    return nullptr;

  Utils::Logger::getInstance().debug("lol");

  /* respond with 204 no content */
  resp->setCode(Server::HTTP::ResponseCode::NO_CONTENT);
  Utils::HttpUtils::basicHeader(resp);
  return resp;
}
} // namespace Lyli::API::Handler