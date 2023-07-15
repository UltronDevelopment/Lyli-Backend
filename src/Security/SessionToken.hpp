// This file is part of the Lyli Backend
// File:    Security/SessionToken.hpp
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

#pragma once

#include <nlohmann/json.hpp>

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>

namespace Lyli::Security {
/* JWT like Session Token */
class SessionToken {
public:
  SessionToken(std::string_view username, std::string_view email,
               std::size_t timestamp);

  explicit SessionToken(std::string_view encrypted_token);

  ~SessionToken() = default;

  const nlohmann::json &asJson() const;

  std::string encryptedToken() const;

  std::string_view getUsername() const;

  std::string_view getEmail() const;

  std::size_t getTimestamp();

private:
  nlohmann::json _json;
};
} // namespace Lyli::Security