// This file is part of the Lyli Backend
// File:    Session.hpp
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

#pragma once

#include <API/Router.hpp>
#include <DB/Client.hpp>
#include <Security/EmailChecker.hpp>
#include <Security/PasswordHasher.hpp>

namespace Lyli {
/* Singleton holding instances of classes to reduce the amount of singletons */
class Session {
public:
  /* get session instance */
  static Session &getInstance();

  /* delet move and copy constructor to ensure there can only be a single
   * instance */
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;
  Session(Session &&) = delete;
  Session &operator=(Session &&) = delete;

  /* get the database client */
  DB::Client &getDatabaseClient();

  /* get the api router */
  API::Router &getApiRouter();

  /* get the email checker */
  Security::EmailChecker &getEmailChecker();

  /* get the password hasher */
  Security::PasswordHasher &getPasswordHasher();

private:
  Session();
  ~Session();

  /* Database Client */
  DB::Client client;

  /* Router */
  API::Router router;

  /* email checker */
  Security::EmailChecker email_checker;

  /* password hasher*/
  Security::PasswordHasher password_hasher;
};
} // namespace Lyli