// This file is part of the Lyli Backend
// File:    DB/Collection.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    16 June 2023
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

#include <DB/Collection.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/Logger.hpp>
#include <Utils/MongoPointer.hpp>

#include <cstddef>
#include <string>

namespace Lyli::DB {
Collection::Collection(const std::string &name, mongoc_database_t *db)
    : name(name), collection(mongoc_database_get_collection(db, name.data())) {}

std::string_view Collection::getName() const { return this->name; }

nlohmann::json Collection::searchDocument(const bson_t *query,
                                          std::uint32_t limit) const {
  Utils::Logger::getInstance().debug("Searching Document");

  auto ret = nlohmann::json::array();
  if (query == nullptr)
    return ret;

  /* set limit search option */
  Utils::BsonPointer::Bson opts{BCON_NEW("limit", BCON_INT32(limit))};

  /* exec query */
  Utils::MongoPointer::Cursor res{mongoc_collection_find_with_opts(
      this->collection.get(), query, opts.get(), nullptr)};

  /* error check */
  if (bson_error_t error; mongoc_cursor_error(res.get(), &error)) {
    Utils::Logger::getInstance().error(error.message);
    return ret;
  }

  /* iterate over bson results and convert it to nlohmann json */
  const bson_t *buffer;
  while (mongoc_cursor_next(res.get(), &buffer)) {
    ret.push_back(bson_as_json(buffer, nullptr));
  }

  return ret;
}

bool Collection::insertDocument(const bson_t *document) const {
  Utils::Logger::getInstance().debug("Inserting Document");

  if (document == nullptr)
    return false;

  if (bson_error_t error;
      !mongoc_collection_insert(this->collection.get(), MONGOC_INSERT_NONE,
                                document, nullptr, &error)) {
    Utils::Logger::getInstance().error(error.message);
    return false;
  }

  return true;
}

Collection::~Collection() = default;
} // namespace Lyli::DB
