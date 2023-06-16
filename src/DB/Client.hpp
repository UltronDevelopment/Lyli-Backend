// This file is part of the Lyli Backend
// File:    DB/Client.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    13 June 2023
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

#include <memory>
#include <mongoc/mongoc.h>

#include <DB/Database.hpp>
#include <Utils/MongoPointer.hpp>

#include <set>
#include <string>
#include <string_view>

namespace Lyli::DB {
class Client {
public:
  static Client &getInstance();

  Client(const Client &) = delete;
  Client &operator=(const Client &) = delete;
  Client(Client &&) = delete;
  Client &operator=(Client &&) = delete;

  void create(const std::string &application_name, std::string_view srv);

  mongoc_client_t *getMongoClient() const;

  bool isValid() const;

  std::shared_ptr<Database> openDB(const std::string &db_name);

  std::shared_ptr<Database> getDatabase(std::string_view db_name) const;

  void close();

private:
  Client();
  ~Client();

  bool valid;

  Utils::MongoPointer::Client client;
  std::set<std::shared_ptr<Database>> databases;
};
} // namespace Lyli::DB
