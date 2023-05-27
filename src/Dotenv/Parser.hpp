// This file is part of the Lyli Backend
// File:    Dotenv/Parser.hpp
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

#include <map>
#include <string>
#include <string_view>

namespace Lyli::Dotenv {
class Parser {
public:
  Parser();
  ~Parser();

  void parse(std::string_view data);

  const std::map<std::string, std::string, std::less<>> &getBuffer() const;

private:
  /* var_name:value */
  std::map<std::string, std::string, std::less<>> vars;
};
} // namespace Lyli::Dotenv