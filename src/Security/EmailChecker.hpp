// This file is part of the Lyli Backend
// File:    Security/EmailChecker.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    21 June 2023
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

#include <regex>

#pragma once

namespace Lyli::Security {
class EmailChecker {
public:
  EmailChecker();
  ~EmailChecker();

  /* validate an email per regex */
  bool check(std::string_view email) const;

private:
  /* email validation regex */
  const std::regex mail_regex{
      R"([a-zA-Z0-9-_.+]{3,}\@[a-z0-9-]{3,}\.[a-z]{2,})",
      std::regex_constants::ECMAScript};
};
} // namespace Lyli::Security