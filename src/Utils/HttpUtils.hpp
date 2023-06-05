// This file is part of the Lyli Backend
// File:    Utils/HttpUtils.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    05 June 2023
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

#include <Server/HTTP/HttpObject.hpp>
#include <Utils/FormattedTime.hpp>

#include <memory>

namespace Lyli::Utils::HttpUtils {

void inline basicHeader(
    const std::shared_ptr<Server::HTTP::HttpObject> &object) {
  object->setHeaderValue("Data", Utils::FormattedTime::HTTP());
  object->setHeaderValue("Connection", "close");
  object->setHeaderValue("Server", "Lyli-Backend");
}
} // namespace Lyli::Utils::HttpUtils