// This file is part of the Lyli Backend
// File:    Dotenv/Parser.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    27 May 2023
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

#include <Dotenv/Parser.hpp>
#include <Utils/Logger.hpp>

#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace Lyli::Dotenv {
Parser::Parser() = default;
Parser::~Parser() = default;

void Parser::parse(std::string_view data) {
  std::size_t nbeg{0};
  std::string name_buffer;

  for (std::size_t i{0}; i < data.size(); i++) {
    /* parse var name */
    if (data.at(i) == '=') {
      name_buffer = std::string(data.data() + nbeg, data.data() + i);
      Utils::Logger::getInstance().debug(
          std::format("Parsed ENV: {}", name_buffer));
    }

    /* parse value for name */
    if (data.at(i) == '"') {
      std::size_t vbeg{++i};
      while (data.at(i) != '"')
        i++;

      this->vars[name_buffer] =
          std::string_view(data.data() + vbeg, data.data() + i - 1);
      Utils::Logger::getInstance().debug(
          std::format("Parsed value from: {}", name_buffer));
    }

    if (data.at(i) == '\n')
      nbeg = i + 1;
  }
}

const std::map<std::string, std::string, std::less<>> &
Parser::getBuffer() const {
  return this->vars;
}
}; // namespace Lyli::Dotenv
