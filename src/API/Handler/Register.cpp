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

#include <nlohmann/json.hpp>

#include <bson/bson.h>

#include <API/Handler/Register.hpp>
#include <DB/Client.hpp>
#include <Server/HTTP/HttpResponse.hpp>
#include <Session.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/DatabaseUtils.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>

#include <cstddef>
#include <cstdlib>
#include <string>

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

bson_t *createUser(nlohmann::json &jval) {

  /* hash password */
  auto hashed_passwd =
      Session::getInstance().getPasswordHasher().hashPasswordForStore(
          jval.at("password"));

  if (hashed_passwd.empty()) {
    return nullptr;
  }

  /* replace hashed password in json */
  jval["password"] = hashed_passwd;

  bson_error_t error;

  return bson_new_from_json(reinterpret_cast<uint8_t *>(jval.dump().data()), -1,
                            &error);
}

const char *check(nlohmann::json &jval) {
  /* check if exists */
  if (!jval.contains("username") || !jval.contains("email") ||
      !jval.contains("password"))
    return "Important field is missing";

  /* check if values are strings */
  if (!jval.at("username").is_string() || !jval.at("password").is_string() ||
      !jval.at("email").is_string())
    return "All values have to be strings";

  /* check username size */
  if (std::size_t size =
          jval.at("username").get_ref<const std::string &>().size();
      size < 4 || size > 20)
    return "Username needs at least 4 character and is not allowed to be "
           "longer than 20 characters";

  /* check password size */
  if (std::size_t size =
          jval.at("password").get_ref<const std::string &>().size();
      size < 8 || size > 25)
    return "password needs at least 8 character and is not allowed to be "
           "greater than 25 characters";

  /* check email */
  if (std::size_t size = jval.at("email").get_ref<const std::string &>().size();
      size < 5 || size > 320)
    return "email needs at least 5 character and is not allowed to be greater "
           "than 320 characters";

  /* validate email */
  if (!Session::getInstance().getEmailChecker().check(
          jval.at("email").get_ref<const std::string &>()))
    return "no valid email";

  /* get user collection */
  const auto user_collection = Utils::DatabaseUtils::getCollectionFromDB(
      std::getenv("DB_NAME"), std::getenv("USER_COLLECTION"));
  if (user_collection == nullptr)
    return "database error";

  /* check if username already exists */
  Utils::BsonPointer::Bson username_query(BCON_NEW(
      "username", jval.at("username").get_ref<const std::string &>().c_str()));

  if (const auto r = user_collection->searchDocument(username_query.get(), 1);
      !r.empty())
    return "username already exists";

  /* check if email already exists */
  Utils::BsonPointer::Bson email_query(BCON_NEW(
      "username", jval.at("email").get_ref<const std::string &>().c_str()));

  if (const auto r = user_collection->searchDocument(email_query.get(), 1);
      !r.empty())
    return "email already exists";

  return nullptr;
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

  if (const char *error = check(json_data); error != nullptr) {
    Utils::HttpUtils::error(resp, error,
                            Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  /* try to create user */
  json_data["isAdmin"] = false;
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