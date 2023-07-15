// This file is part of the Lyli Backend
// File:    Security/SessionToken.cpp
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

#include <Security/KeyPair.hpp>
#include <Security/SessionToken.hpp>
#include <Session.hpp>
#include <Utils/StringUtils.hpp>

namespace Lyli::Security {
SessionToken::SessionToken(std::string_view username, std::string_view email,
                           std::size_t timestamp) {
  nlohmann::json json;
  json["username"] = username;
  json["email"] = email;
  json["timestamp"] = timestamp;
}

SessionToken::SessionToken(std::string_view encrypted_token) {

  /* convert bytes represantet as string to raw bytes */
  const auto bytes = Utils::StringUtils::bytestringToBytes(encrypted_token);
  if (bytes.empty())
    return;

  const auto dec = Session::getInstance().getKeyPair().decrypt(bytes);
  if (dec.empty())
    return;

  this->_json = nlohmann::json::parse(dec);
}

const nlohmann::json &SessionToken::asJson() const { return this->_json; }

std::string SessionToken::encryptedToken() const {
  return Session::getInstance().getKeyPair().encrypt(this->_json.dump());
}

std::string_view SessionToken::getUsername() const {
  if (!this->_json.contains("username"))
    return "";

  return this->_json.at("username").get_ref<const std::string &>();
}

std::string_view SessionToken::getEmail() const {
  if (!this->_json.contains("email"))
    return "";

  return this->_json.at("email").get_ref<const std::string &>();
}

std::size_t SessionToken::getTimestamp() {
  if (!this->_json.contains("timestamp"))
    return 0;

  const auto &str = this->_json.at("username").get_ref<const std::string &>();

  /* check if string is not empty and numerical */
  if (str.empty() || std::find_if(str.begin(), str.end(), [](unsigned char c) {
                       return !std::isdigit(c);
                     }) != str.end())
    return 0;

  return strtoul(str.c_str(), nullptr, 10);
}

} // namespace Lyli::Security