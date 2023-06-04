// This file is part of the Lyli Backend
// File:    Utils/FormattedTime.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    04 June 2023
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

#include <Utils/FormattedTime.hpp>

#include <chrono>
#include <ctime>
#include <locale>

/* https://en.cppreference.com/w/cpp/chrono/duration/formatter */
namespace Lyli::Utils {
static auto getTime() {
  return std::chrono::current_zone()->to_local(
      std::chrono::high_resolution_clock::now());
}

std::string FormattedTime::HTTP() {
  return std::format("{:%a, %d %b %T %Z}", std::chrono::system_clock::now());
}

std::string FormattedTime::Logger() {
  return std::format("{:%Y-%m-%d %X}", getTime());
}
} // namespace Lyli::Utils
