// This file is part of the Lyli Backend
// File:    Dotenv/Loader.cpp
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

#include <Dotenv/Loader.hpp>
#include <Dotenv/Parser.hpp>
#include <Utils/Logger.hpp>

#include <cstddef>
#include <cstdlib>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>

namespace Lyli::Dotenv {
Loader::Loader() = default;
Loader::~Loader() = default;

void Loader::load(const std::string &path) const {
  /* open file stream for reading */
  std::ifstream file{path, std::ios::in};
  if (!file.is_open()) {
    Utils::Logger::getInstance().error(std::format("failed to open {}", path));
    return;
  }

  /* load file content into buffer */
  std::string buffer((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

  /* if buffer is empty return */
  if (buffer.empty()) {
    Utils::Logger::getInstance().error("buffer after .env reading is empty");
    return;
  }

  /* load into environment */
  this->loadIntoEnv(buffer);
}

void Loader::loadIntoEnv(std::string_view data) const {
  Parser parser{};
  parser.parse(data);

  /* create a environment variable with the key as name and the value as value
   for every entry in our parser map */
  for (const auto &[key, value] : parser.getBuffer()) {
    setenv(key.data(), value.data(), 1);
  }
}
} // namespace Lyli::Dotenv
