// This file is part of the Lyli Backend
// File:    Security/EmailChecker.cpp
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

#include <Security/EmailChecker.hpp>

#include <regex>

namespace Lyli::Security {
EmailChecker::EmailChecker() = default;

EmailChecker::~EmailChecker() = default;

bool EmailChecker::check(std::string_view email) const {
  return std::regex_match(email.data(), this->mail_regex);
}

} // namespace Lyli::Security