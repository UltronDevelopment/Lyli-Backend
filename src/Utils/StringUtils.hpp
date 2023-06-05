// This file is part of the Lyli Backend
// File:    Utils/StringUtils.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    30 May 2023
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

#include <string_view>
#include <algorithm>

namespace Lyli::Utils::StringUtils {
/* get the position from the next byte with the value @c in @str as iterator */
inline const char *ptrToNext(std::string_view str, char c) {
  if (str.begin() + 1 == str.end())
    return nullptr;

  auto it = std::find(str.begin(), str.end(), c);
  if (it == str.end())
    return nullptr;

  return it;
}
} // namespace Lyli::Utils::StringUtils
