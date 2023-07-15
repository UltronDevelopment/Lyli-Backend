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

#include <bits/types/time_t.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

/* https://en.cppreference.com/w/cpp/chrono/duration/formatter */
namespace Lyli::Utils {

static time_t getTime() {
  return std::chrono::high_resolution_clock::to_time_t(
      std::chrono::high_resolution_clock::now());
}

std::string FormattedTime::HTTP() {
  std::stringstream buffer;

  auto time = getTime();
  const auto *tm = std::localtime(&time);
  buffer << std::put_time(tm, "%a, %d %b %T %Z");
  return buffer.str();
}

std::string FormattedTime::Logger() {
  std::stringstream buffer;

  auto time = getTime();
  const auto *tm = std::localtime(&time);
  buffer << std::put_time(tm, "%Y %b %d %X");
  return buffer.str();
}

std::size_t FormattedTime::UNIX() {
  const auto tp = std::chrono::system_clock::now();
  const auto time =
      std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch())
          .count();

  return time;
}
} // namespace Lyli::Utils
