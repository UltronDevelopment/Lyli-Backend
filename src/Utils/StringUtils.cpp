// This file is part of the Lyli Backend
// File:    Utils/StringUtils.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    15 July 2023
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

#include <Utils/StringUtils.hpp>

#include <array>
#include <cstdlib>
#include <iomanip>
#include <sstream>

namespace Lyli::Utils::StringUtils {
std::string bytesToString(const std::vector<std::uint8_t> &data) {
  std::stringstream buffer;
  buffer << std::hex;

  for (std::uint8_t byte : data) {
    buffer << std::setw(2) << std::setfill('0')
           << static_cast<std::uint16_t>(byte);
  }

  return buffer.str();
}

std::vector<std::uint8_t> bytestringToBytes(std::string_view data) {
  std::vector<std::uint8_t> buffer;

  if ((data.size() % 2) != 0) {
    return std::vector<std::uint8_t>();
  }

  buffer.reserve(data.size() / 2);

  std::array<char, 3> tmp{0, 0, 0};
  for (std::size_t i{0}; i < data.size(); i++) {
    tmp[0] = data.at(i);
    tmp[1] = data.at(++i);

    buffer.push_back(
        static_cast<std::uint8_t>(std::strtoul(tmp.data(), nullptr, 16)));
  }

  return buffer;
}
} // namespace Lyli::Utils::StringUtils