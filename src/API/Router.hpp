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
#include <Server/HTTP/HttpResponse.hpp>

#include <functional>
#include <map>
#include <string_view>

#pragma once

namespace Lyli::API {
class Router {
public:
  using RequestHandler =
      std::function<std::shared_ptr<Server::HTTP::HttpResponse>(
          const std::shared_ptr<Server::HTTP::HttpRequest> &request)>;

  Router();
  ~Router();

  void setup();

  /* set all routes to their handler */
  RequestHandler route(std::string_view path) const;

private:
  bool addRoute(std::string_view route, const RequestHandler &handler);

  static std::shared_ptr<Server::HTTP::HttpResponse>
  notFound(const std::shared_ptr<Server::HTTP::HttpRequest> &request);

  std::map<std::string_view, RequestHandler, std::less<>> routes;
};
} // namespace Lyli::API