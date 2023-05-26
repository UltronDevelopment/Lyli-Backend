// File:    Utils/Logger.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    26 May 2032
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
// (c) UPN
//

#include <Utils/Logger.hpp>

#include <chrono>
#include <iostream>

namespace Lyli::Utils {
Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::trace(std::string_view txt) const {
  if (LOG_TRACE) {
    for (auto *stream : this->streams) {
      if (stream != nullptr && stream->good())
        (*stream) << this->getTimeDate() << " | " << txt << std::endl;
    }
  }
}

void Logger::debug(std::string_view txt) const {
  if (LOG_DEBUG) {
    for (auto *stream : this->streams) {
      if (stream != nullptr && stream->good())
        (*stream) << this->getTimeDate() << " | " << txt << std::endl;
    }
  }
}

std::string Logger::getTimeDate() const {
  auto const time =
      std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
  return std::format("{:%Y-%m-%d %X}", time);
}

Logger::Logger() { streams.push_back(&std::cout); }
} // namespace Lyli::Utils