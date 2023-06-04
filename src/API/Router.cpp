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

#include <API/Router.hpp>
#include <Utils/Logger.hpp>
#include <functional>

namespace Lyli::API {
Router &Router::getInstance() {
  static Router instance;
  return instance;
}

Router::Router() = default;

std::string
lol([[maybe_unused]] const std::shared_ptr<Server::HTTP::HttpRequest>
        &request) {
  return "Deine Mama";
}

void Router::setup() {
  const auto &logger = Utils::Logger::getInstance();

  /* /test */
  if (!routes.try_emplace("/test", &lol).second)
    logger.error("failed to create route: /test");
}

Router::Handler Router::route(std::string_view path) const {
  if (this->routes.contains(path))
    return this->routes.at(path);

  return &Router::notFound;
}

std::string Router::notFound(
    [[maybe_unused]] const std::shared_ptr<Server::HTTP::HttpRequest>
        &request) {
  return "Not Found";
}
} // namespace Lyli::API