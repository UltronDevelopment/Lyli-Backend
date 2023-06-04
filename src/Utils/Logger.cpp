// This file is part of the Lyli Backend
// File:    Utils/Logger.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    26 May 2023
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
#include <Utils/Logger.hpp>

#include <chrono>
#include <iostream>

namespace Lyli::Utils {
Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::trace([[maybe_unused]] std::string_view txt) const {
  if (LOG_TRACE) {
    for (auto *stream : this->streams) {
      if (stream != nullptr && stream->good())
        (*stream) << FormattedTime::Logger() << " | " << txt << std::endl;
    }
  }
}

void Logger::debug([[maybe_unused]] std::string_view txt) const {
  if (LOG_DEBUG) {
    for (auto *stream : this->streams) {
      if (stream != nullptr && stream->good())
        (*stream) << FormattedTime::Logger() << " | [DEBUG] " << txt
                  << std::endl;
    }
  }
}

void Logger::error(std::string_view txt) const {
  for (auto *stream : this->streams) {
    if (stream != nullptr && stream->good())
      (*stream) << FormattedTime::Logger() << " | [ERROR] " << txt << std::endl;
  }
}

Logger::Logger() { streams.push_back(&std::cout); }
} // namespace Lyli::Utils
