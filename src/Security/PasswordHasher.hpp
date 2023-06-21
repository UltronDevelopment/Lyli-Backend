// This file is part of the Lyli Backend
// File:    Security/PasswordHasher.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    18 June 2023
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

#pragma once

#include <openssl/sha.h>

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

namespace Lyli::Security {
class PasswordHasher {
public:
  PasswordHasher();
  ~PasswordHasher();

  /* hash a password with random salt and pepper and get the hash thats used to
   * be stored in a DB */
  std::string hashPasswordForStore(std::string password) const;

  /* hash a password with a custom salt and pepper */
  std::string hashPassword(std::string password, std::string_view salt,
                           char pepper) const;

  /* remove the salt from the stored hash and return it */
  std::string extractSalt(std::string &hash) const;

  /* get the configured pepper range */
  const std::pair<char, char> &getPepperRange() const;

private:
  /* generate a sha1 hash */
  std::pair<std::array<std::uint8_t, SHA_DIGEST_LENGTH>, bool>
  sha1(const std::string &data) const;

  /* convert a sha1 hash to a readable string */
  std::string
  sha1ToString(const std::array<std::uint8_t, SHA_DIGEST_LENGTH> &hash) const;

  /* generate a random {salt_lenght} byte long salt string */
  std::string saltGen() const;

  /* get a random byte from our pepper range */
  char getRandomPepper() const;

  /* generate a random number from min to max */
  int randomNumber(int min, int max) const;

  /* salt string lenght */
  static constexpr int salt_lenght{6};

  /* determines where we store our salt in the hash, before saving the
   * password
   */
  std::size_t salt_insert{0};

  /* possible pepper range (from low to high) */
  std::pair<char, char> pepper_range;
};
} // namespace Lyli::Security