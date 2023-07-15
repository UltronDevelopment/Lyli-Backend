// This file is part of the Lyli Backend
// File:    API/Handler/User/UserCheck.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    15 July 2023
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

#include <API/Handler/User/UserCheck.hpp>
#include <Utils/DatabaseUtils.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>

#include <string>
#include <string_view>

namespace Lyli::API::Handler::User {
std::shared_ptr<Server::HTTP::HttpResponse>
getUserInfo(const std::shared_ptr<Server::HTTP::HttpRequest> &request) {
  const auto resp = Server::HTTP::HttpResponse::create();
  if (resp == nullptr)
    return nullptr;

  Utils::HttpUtils::basicHeader(resp);

  /* if request type is not POST send 405 method not allowed */
  if (request->getRequestType() != Server::HTTP::RequestType::POST) {
    resp->setCode(Server::HTTP::ResponseCode::METHOD_NOT_ALLOWED);
    return resp;
  }

  /* no data no response */
  if (request->getData().empty()) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  /* obtain request data and store into json object */
  nlohmann::basic_json json_data;
  try {
    json_data = nlohmann::json::parse(request->getData());
  } catch ([[maybe_unused]] nlohmann::json::parse_error &error) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    resp->setHeaderValue("Content-Type", "application/json");
    resp->setData(R"({"error": "no valid json data"})");
    return resp;
  }

  if (json_data.is_null() || json_data.empty() ||
      !json_data.contains("auth-token")) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  Security::SessionToken st{
      json_data.at("auth-token").get_ref<const std::string &>()};

  /* check if token is invalid */
  if (st.getTimestamp() <= Utils::FormattedTime::UNIX()) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    resp->setHeaderValue("Content-Type", "application/json");
    resp->setData(R"({"error": "token is invalid"})");
    return resp;
  }

  auto user = Utils::DatabaseUtils::getUserFromToken(st);
  if (user.empty()) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    resp->setHeaderValue("Content-Type", "application/json");
    resp->setData(R"({"error": "token is invalid"})");
    return resp;
  }

  user = user.at(0);

  if (user.empty() || !user.contains("username") || !user.contains("isAdmin")) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  nlohmann::json resp_data;
  resp_data["username"] = user.at("username").get_ref<const std::string &>();
  resp_data["isAdmin"] = user.at("isAdmin").get<bool>();

  resp->setCode(Server::HTTP::ResponseCode::OK);
  resp->setData(resp_data.dump());
  return resp;
}
} // namespace Lyli::API::Handler::User