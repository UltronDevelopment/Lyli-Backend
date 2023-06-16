// This file is part of the Lyli Backend
// File:    DB/Client.cpp
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

#include <DB/Client.hpp>
#include <Utils/Logger.hpp>

#include <algorithm>
#include <memory>

namespace Lyli::DB {
Client &Client::getInstance() {
  static Client instance;
  return instance;
}

void Client::create(const std::string &application_name, std::string_view srv) {
  Utils::MongoPointer::Uri uri{mongoc_uri_new(srv.data())};
  if (uri == nullptr) {
    Utils::Logger::getInstance().error("URI is not valid");
    return;
  }

  /* create client from srv */
  Utils::Logger::getInstance().trace("Creating MongoDB Client");
  this->client.reset(mongoc_client_new_from_uri(uri.get()));

  /* set application name for better server logging */
  if (!mongoc_client_set_appname(this->client.get(),
                                 application_name.c_str())) {
    Utils::Logger::getInstance().error("Failed setting application name");
    return;
  }

  Utils::Logger::getInstance().trace("MongoDB Appname: " + application_name);

  this->valid = true;
}

mongoc_client_t *Client::getMongoClient() const { return this->client.get(); }

bool Client::isValid() const { return this->valid; }

std::shared_ptr<Database> Client::openDB(const std::string &db_name) {
  auto [it,
        succeed]{this->databases.insert(std::make_shared<Database>(db_name))};

  return succeed ? *it : nullptr;
}

std::shared_ptr<Database> Client::getDatabase(std::string_view db_name) const {
  auto it{std::find_if(this->databases.cbegin(), this->databases.cend(),
                       [&db_name](const std::shared_ptr<Database> &db) {
                         return db->getName() == db_name;
                       })};

  if (it == this->databases.cend())
    return nullptr;

  return *it;
}

void Client::close() {
  Utils::Logger::getInstance().debug("DB Driver Cleanup");
  mongoc_cleanup();
  this->valid = false;
}

Client::Client() { mongoc_init(); };

Client::~Client() { this->close(); };
} // namespace Lyli::DB
