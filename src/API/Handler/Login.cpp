// This file is part of the Lyli Backend
// File:    API/Handler/Login.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    22 June 2023
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

#include <API/Handler/Login.hpp>
#include <Security/PasswordHasher.hpp>
#include <Server/HTTP/HttpResponse.hpp>
#include <Session.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/DatabaseUtils.hpp>
#include <Utils/HttpUtils.hpp>
#include <Utils/Logger.hpp>

#include <cstdlib>
#include <memory>
#include <openssl/sha.h>
#include <string>

namespace Lyli::API::Handler {

/* validate the password */
bool validatePassword(std::string_view password, std::string db_hash) {
  /* get password hasher and extract the salt, from the database hash*/
  const auto &pwh = Session::getInstance().getPasswordHasher();
  const auto salt = pwh.extractSalt(db_hash);

  if (salt.size() != Security::PasswordHasher::salt_lenght ||
      db_hash.size() != SHA_DIGEST_LENGTH * 2)
    return false;

  /* pepper salted hash and check if one of the hashes match */
  const auto [pepper_start, pepper_end] = pwh.getPepperRange();
  for (char pepper = pepper_start; pepper <= pepper_end; pepper++) {
    const auto &pw_hash = pwh.hashPassword(password.data(), salt, pepper);
    if (pw_hash == db_hash)
      return true;
  }

  return false;
}

/* returns string containing error or a nullptr on success */
const char *validateData(const nlohmann::json &jval) {
  /* check if username and password exist in request and are not empty */
  if (!jval.contains("email") || !jval.contains("password") ||
      jval.at("email").get_ref<const std::string &>().empty() ||
      jval.at("password").get_ref<const std::string &>().empty())
    return "email and password are required to login";

  /* get user collection from database */
  const auto collection = Utils::DatabaseUtils::getCollectionFromDB(
      std::getenv("DB_NAME"), std::getenv("USER_COLLECTION"));

  if (collection == nullptr)
    return "database error";

  Utils::BsonPointer::Bson query(BCON_NEW(
      "email", jval.at("email").get_ref<const std::string &>().c_str()));
  auto user = collection->searchDocument(query.get(), 1);
  if (user.empty())
    return "wrong email or password";

  user = user.at(0);

  if (!user.contains("email") || !user.contains("username") ||
      !user.contains("password"))
    return "database error";

  if (!validatePassword(jval.at("password").get_ref<const std::string &>(),
                        user.at("password").get_ref<const std::string &>()))
    return "wrong email or password";

  return nullptr;
}

std::shared_ptr<Server::HTTP::HttpResponse>
login(const std::shared_ptr<Server::HTTP::HttpRequest> &request) {
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

  if (const auto *err = validateData(json_data); err != nullptr) {
    Utils::HttpUtils::error(resp, err, Server::HTTP::ResponseCode::BAD_REQUEST);
    return resp;
  }

  resp->setCode(Server::HTTP::ResponseCode::OK);
  resp->setData("Login Successful");
  return resp;
}
} // namespace Lyli::API::Handler
