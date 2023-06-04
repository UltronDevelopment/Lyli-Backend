// This file is part of the Lyli Backend
// File:    API/Router.hpp
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

#include <Server/HTTP/HttpRequest.hpp>

#include <functional>
#include <map>
#include <string_view>

namespace Lyli::API {
class Router {
public:
  using Handler = std::string (*)(
      const std::shared_ptr<Server::HTTP::HttpRequest> &request);

  static Router &getInstance();

  void setup();

  /* set all routes to their handler */
  Handler route(std::string_view path) const;

  /* delete move and copy constructor to ensure there can only be a single
   * instance */
  Router(const Router &) = delete;
  Router &operator=(const Router &) = delete;
  Router(Router &&) = delete;
  Router &operator=(Router &&) = delete;

private:
  Router();

  static std::string
  notFound(const std::shared_ptr<Server::HTTP::HttpRequest> &request);

  std::map<std::string_view, Handler, std::less<>> routes;
};
} // namespace Lyli::API