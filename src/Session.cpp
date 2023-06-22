// This file is part of the Lyli Backend
// File:    Session.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    20 June 2023
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

#include <Session.hpp>

namespace Lyli {
Session &Session::getInstance() {
  static Session instance;
  return instance;
}

DB::Client &Session::getDatabaseClient() { return this->client; }

API::Router &Session::getApiRouter() { return this->router; }

Security::EmailChecker &Session::getEmailChecker() {
  return this->email_checker;
}

Security::PasswordHasher &Session::getPasswordHasher() {
  return this->password_hasher;
}

Session::Session() = default;
Session::~Session() = default;
} // namespace Lyli