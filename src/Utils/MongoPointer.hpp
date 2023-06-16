// This file is part of the Lyli Backend
// File:    Utils/MongoPointer.hpp
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

#include <mongoc/mongoc.h>

#include <memory>

namespace Lyli::Utils::MongoPointer {
using ClientDelete = struct ClientDelete {
  void operator()(mongoc_client_t *x) const { mongoc_client_destroy(x); }
};

using DatabaseDelete = struct DatabaseDelete {
  void operator()(mongoc_database_t *x) const { mongoc_database_destroy(x); }
};

using CollectionDelete = struct CollectionDelete {
  void operator()(mongoc_collection_t *x) const {
    mongoc_collection_destroy(x);
  }
};

using UriDelete = struct UriDelete {
  void operator()(mongoc_uri_t *x) const { mongoc_uri_destroy(x); }
};

using CursorDelete = struct CursorDelete {
  void operator()(mongoc_cursor_t *x) const { mongoc_cursor_destroy(x); }
};

using Client = std::unique_ptr<mongoc_client_t, ClientDelete>;
using Database = std::unique_ptr<mongoc_database_t, DatabaseDelete>;
using Collection = std::unique_ptr<mongoc_collection_t, CollectionDelete>;
using Uri = std::unique_ptr<mongoc_uri_t, UriDelete>;
using Cursor = std::unique_ptr<mongoc_cursor_t, CursorDelete>;
} // namespace Lyli::Utils::MongoPointer
