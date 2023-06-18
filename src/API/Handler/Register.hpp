// This file is part of the Lyli Backend
// File:    API/Handler/Register.hpp
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

#include <nlohmann/json.hpp>

#include <bson/bson.h>

#include <Server/HTTP/HttpRequest.hpp>
#include <Server/HTTP/HttpResponse.hpp>

#include <memory>

namespace Lyli::API::Handler {

bool addUser(const bson_t *document);

bson_t *createUser(const nlohmann::json &jval);

std::shared_ptr<Server::HTTP::HttpResponse>
handle_register(const std::shared_ptr<Server::HTTP::HttpRequest> &request);
} // namespace Lyli::API::Handler