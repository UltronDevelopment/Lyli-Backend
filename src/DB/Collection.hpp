// This file is part of the Lyli Backend
// File:    DB/Collection.hpp
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

#pragma once

#include <nlohmann/json.hpp>

#include <mongoc/mongoc.h>

#include <Utils/MongoPointer.hpp>

#include <string_view>

namespace Lyli::DB {
class Collection {
public:
  explicit Collection(std::string_view name, mongoc_database_t *db);
  ~Collection();

  std::string_view getName() const;

  nlohmann::json searchDocument(const bson_t *query, std::uint32_t limit) const;

  bool insertDocument(const bson_t *document) const;

private:
  std::string_view name;
  Utils::MongoPointer::Collection collection;
};
} // namespace Lyli::DB
