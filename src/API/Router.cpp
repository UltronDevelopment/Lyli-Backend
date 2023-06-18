// This file is part of the Lyli Backend
// File:    API/Router.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    27 May 2023
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
#include <API/Handler/Register.hpp>
#include <API/Router.hpp>
#include <Server/HTTP/HttpResponse.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>

#include <functional>
#include <memory>
#include <string>
#include <string_view>

namespace Lyli::API {
Router &Router::getInstance() {
  static Router instance;
  return instance;
}

Router::Router() = default;

bool Router::addRoute(std::string_view route, const RequestHandler &handler) {
  return !routes.try_emplace(route, handler).second;
}

void Router::setup() {
  const auto &logger = Utils::Logger::getInstance();

  /* /ping */
  if (this->addRoute("/ping", Handler::ping))
    logger.error("failed to create route: /ping");

  /* /register */
  if (this->addRoute("/register", Handler::handle_register))
    logger.error("failed to create route: /register");

  Utils::Logger::getInstance().debug("Router setup completed");
}

Router::RequestHandler Router::route(std::string_view path) const {
  if (this->routes.contains(path))
    return this->routes.at(path);

  return &Router::notFound;
}

std::shared_ptr<Server::HTTP::HttpResponse> Router::notFound([
    [maybe_unused]] const std::shared_ptr<Server::HTTP::HttpRequest> &request) {
  auto resp = Server::HTTP::HttpResponse::create();
  if (resp == nullptr) {
    return nullptr;
  }

  std::string_view err_msg(R"({ "error": "Path not found" })");

  resp->setCode(Server::HTTP::ResponseCode::NOT_FOUND);
  Utils::HttpUtils::basicHeader(resp);

  resp->setHeaderValue("Content-Type", "application/json");
  resp->setHeaderValue("Content-Lenght", std::to_string(err_msg.size()));
  resp->setData(err_msg);

  return resp;
}
} // namespace Lyli::API