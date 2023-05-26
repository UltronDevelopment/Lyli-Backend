// File:    Utils/Logger.hpp
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

#pragma once

#include <fstream>
#include <string_view>
#include <vector>

namespace Lyli::Utils {
class Logger {
public:
  static Logger &getInstance();

  void trace(std::string_view txt) const;

  void debug(std::string_view txt) const;

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &&) = delete;

private:
  std::string getTimeDate() const;

  Logger();

  std::vector<std::ostream *> streams;
};
} // namespace Lyli::Utils