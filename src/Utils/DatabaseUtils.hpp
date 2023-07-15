// This file is part of the Lyli Backend
// File:    Utils/BsonPointer.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    17 June 2023
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
#include <DB/Database.hpp>
#include <Security/SessionToken.hpp>
#include <Session.hpp>
#include <Utils/BsonPointer.hpp>
#include <Utils/Logger.hpp>

#include <cstdlib>
#include <memory>

namespace Lyli::Utils::DatabaseUtils {
inline std::shared_ptr<DB::Collection>
getCollectionFromDB(std::string_view DB_NAME,
                    std::string_view COLLECTION_NAME) {
  const auto &client{Session::getInstance().getDatabaseClient()};
  if (!client.isValid())
    return nullptr;

  auto db{client.getDatabase(DB_NAME)};
  if (db == nullptr)
    return nullptr;

  return db->getCollection(COLLECTION_NAME);
}

inline nlohmann::json getUserFromToken(const Security::SessionToken &token) {
  const auto collection = getCollectionFromDB(std::getenv("DB_NAME"),
                                              std::getenv("USER_COLLECTION"));

  if (collection == nullptr || token.getUsername().empty())
    return {};

  /* search for user in db by the given email */
  Utils::BsonPointer::Bson user_query(
      BCON_NEW("email", token.getEmail().data()));
  return collection->searchDocument(user_query.get(), 1);
}
} // namespace Lyli::Utils::DatabaseUtils
