// This file is part of the Lyli Backend
// File:    DB/Database.cpp
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

#include <DB/Collection.hpp>
#include <Session.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/Logger.hpp>

#include <memory>

namespace Lyli::DB {
Database::Database(const std::string &name)
    : name(name),
      database(mongoc_client_get_database(
          Session::getInstance().getDatabaseClient().getMongoClient(),
          name.c_str())) {
  /* according to the API we will always get an DB (we create one if @name
   * dosn't exists), but just to be sure we dont segfault */
  if (database.get() == nullptr) {
    Utils::Logger::getInstance().error("Failed to get Database: " + name);
  }

  Utils::Logger::getInstance().debug("Get handle for Database: " + name);

  this->ping();
}

bool Database::ping() const {

  /* hacky bullshit */
  bson_t reply;
  Utils::BsonPointer::Bson reply_ptr{&reply};
  bson_error_t error{.code = 0};

  Utils::Logger::getInstance().debug("PING " + this->name);

  if (Utils::BsonPointer::Bson command{BCON_NEW("ping", BCON_INT32(1))};
      mongoc_database_command_simple(database.get(), command.get(), nullptr,
                                     &reply, &error)) {
    return true;
  }

  Utils::Logger::getInstance().error(error.message);

  return false;
}

void Database::openCollection(const std::string &collection_name) {
  this->collections.push_back(
      std::make_shared<Collection>(collection_name, this->database.get()));
}

std::shared_ptr<Collection>
Database::getCollection(std::string_view collection_name) const {
  Utils::Logger::getInstance().debug(collection_name);
  auto it{
      std::find_if(this->collections.cbegin(), this->collections.cend(),
                   [&collection_name](const std::shared_ptr<Collection> &c) {
                     Utils::Logger::getInstance().debug(c->getName());
                     return c->getName() == collection_name;
                   })};

  if (it == this->collections.cend())
    return nullptr;

  return *it;
}

std::string_view Database::getName() const { return this->name; }

Database::~Database() = default;
} // namespace Lyli::DB
