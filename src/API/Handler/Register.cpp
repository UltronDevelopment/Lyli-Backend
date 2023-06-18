// This file is part of the Lyli Backend
// File:    API/Handler/Register.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    17 June 2023
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

#include <bson/bson.h>

#include <API/Handler/Register.hpp>
#include <DB/Client.hpp>
#include <Server/HTTP/HttpResponse.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/DatabaseUtils.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>

#include <cstddef>
#include <cstdlib>
#include <nlohmann/json.hpp>

namespace Lyli::API::Handler {
bool addUser(const bson_t *document) {
  /* get user collection */
  auto collection{Utils::DatabaseUtils::getCollectionFromDB(
      std::getenv("DB_NAME"), std::getenv("USER_COLLECTION"))};
  if (collection == nullptr) {
    return false;
  }
  /* try to add user to db and return status */
  return collection->insertDocument(document);
}

bson_t *createUser(const nlohmann::json &jval) {
  if (!jval.contains("username") || !jval.contains("email") ||
      !jval.contains("password"))
    return nullptr;

  bson_error_t error;

  return bson_new_from_json(reinterpret_cast<uint8_t *>(jval.dump().data()), -1,
                            &error);
}

std::shared_ptr<Server::HTTP::HttpResponse>
handle_register(const std::shared_ptr<Server::HTTP::HttpRequest> &request) {
  auto resp = Server::HTTP::HttpResponse::create();
  if (resp == nullptr)
    return nullptr;

  /* fill out basic header stuff */
  Utils::HttpUtils::basicHeader(resp);

  /* if request type is not POST send method not allowed */
  if (request->getRequestType() != Server::HTTP::RequestType::POST) {
    resp->setCode(Server::HTTP::ResponseCode::METHOD_NOT_ALLOWED);
    return resp;
  }

  /* no data no user */
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

  if (json_data.is_null() || json_data.empty()) {
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  /* try to create user */
  if (Utils::BsonPointer::Bson document{createUser(json_data)};
      !addUser(document.get())) {
    Utils::Logger::getInstance().error("Failed to register");
    resp->setCode(Server::HTTP::ResponseCode::BAD_REQUEST);
    resp->setData(R"({"error": "failed to register"})");
    return resp;
  }

  resp->setCode(Server::HTTP::ResponseCode::OK);
  return resp;
}
} // namespace Lyli::API::Handler