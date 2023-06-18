// This file is part of the Lyli Backend
// File:    DB/Database.hpp
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

#include <DB/Collection.hpp>
#include <Utils/MongoPointer.hpp>

#include <memory>
#include <vector>

namespace Lyli::DB {
class Database {
public:
  explicit Database(const std::string &name);
  ~Database();

  bool ping() const;

  void openCollection(const std::string &collection_name);

  std::shared_ptr<Collection>
  getCollection(std::string_view collection_name) const;

  std::string_view getName() const;

private:
  std::string name;
  Utils::MongoPointer::Database database;
  std::vector<std::shared_ptr<Collection>> collections;
};
} // namespace Lyli::DB
