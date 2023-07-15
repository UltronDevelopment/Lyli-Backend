// This file is part of the Lyli Backend
// File:    Security/PasswordHasher.cpp
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

#include <openssl/sha.h>

#include <Security/PasswordHasher.hpp>
#include <Utils/Logger.hpp>

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace Lyli::Security {
PasswordHasher::PasswordHasher() {
  /* get our salt insert value from the environment (0 if nullptr)*/
  Utils::Logger::getInstance().debug("Get SALT_CFG");
  const char *salt_str{std::getenv("SALT_CFG")};
  salt_insert = !salt_str ? 0 : strtol(salt_str, nullptr, 10);

  Utils::Logger::getInstance().debug("Salt Config: " +
                                     std::to_string(salt_insert));

  /* get pepper range from environment (or use default, whenn nullptr)*/
  Utils::Logger::getInstance().debug("Get PEPPER_RANGE");
  const char *pepper_str{std::getenv("PEPPER_RANGE")};
  if (!pepper_str) {
    Utils::Logger::getInstance().error("Using default Pepper Range");
    pepper_str = "AG";
  }

  this->pepper_range.first = pepper_str[0];
  this->pepper_range.second = pepper_str[1];

  /* if first is not greater that second, use default values */
  if (pepper_range.first > this->pepper_range.second) {
    Utils::Logger::getInstance().error("Using default Pepper Range");
    this->pepper_range.first = 'A';
    this->pepper_range.first = 'G';
  }

  Utils::Logger::getInstance().debug(
      "Pepper Range: " + std::to_string(pepper_range.first) + " -> " +
      std::to_string(pepper_range.second));

  Utils::Logger::getInstance().trace("Configured PasswordHasher");
}

PasswordHasher::~PasswordHasher() = default;

std::string PasswordHasher::hashPasswordForStore([
    [maybe_unused]] std::string password) const {

  /* append salt and pepper */
  auto salt = this->saltGen();
  password.append(salt);
  password.insert(password.end(), this->getRandomPepper());

  /* hash password */
  const auto &[hash, succeeded] = this->sha1(password);
  if (!succeeded)
    return "";

  auto hash_string = this->sha1ToString(hash);

  /* store salt in hased password */
  hash_string.insert(salt_insert, salt);

  return hash_string;
}

std::string PasswordHasher::hashPassword(std::string password,
                                         std::string_view salt,
                                         char pepper) const {
  password.append(salt);
  password.insert(password.cend(), pepper);

  /* hash password */
  const auto &[hash, succeeded] = this->sha1(password);
  if (!succeeded)
    return "";

  return this->sha1ToString(hash);
}

std::pair<std::array<std::uint8_t, SHA_DIGEST_LENGTH>, bool>
PasswordHasher::sha1(const std::string &data) const {
  std::array<std::uint8_t, SHA_DIGEST_LENGTH> buffer;

  SHA_CTX ctx;
  if (!SHA1_Init(&ctx)) {
    Utils::Logger::getInstance().error("OpenSSL Failed SHA_INIT");
    return {buffer, false};
  }

  if (!SHA1_Update(&ctx, data.c_str(), data.size())) {
    Utils::Logger::getInstance().error("OpenSSL Failed SHA_Update");
    return {buffer, false};
  }

  if (!SHA1_Final(buffer.data(), &ctx)) {
    Utils::Logger::getInstance().error("OpenSSL Failed SHA_Final");
    return {buffer, false};
  }

  return {buffer, true};
}

std::string PasswordHasher::extractSalt(std::string &hash) const {
  const auto salt_begin = hash.begin() + salt_insert;
  const auto salt_end = hash.begin() + salt_insert + salt_lenght;
  std::string salt(salt_begin, salt_end);
  hash.erase(salt_begin, salt_end);

  return salt;
}

const std::pair<char, char> &PasswordHasher::getPepperRange() const {
  return this->pepper_range;
}

std::string PasswordHasher::sha1ToString(
    const std::array<std::uint8_t, SHA_DIGEST_LENGTH> &hash) const {
  std::stringstream buffer;
  buffer << std::hex;

  for (std::uint8_t byte : hash) {
    buffer << std::setw(2) << std::setfill('0')
           << static_cast<std::uint16_t>(byte);
  }

  return buffer.str();
}

std::string PasswordHasher::saltGen() const {
  auto randchar = [this]() {
    constexpr char charset[] = "0123456789"
                               "abcdef";
    constexpr size_t max_index = (sizeof(charset) - 1);
    return charset[this->randomNumber(0, max_index)];
  };

  std::string str(salt_lenght, 0);
  std::generate_n(str.begin(), salt_lenght, randchar);
  return str;
}

char PasswordHasher::getRandomPepper() const {
  return randomNumber(this->pepper_range.first, this->pepper_range.second) &
         0xFF;
}

int PasswordHasher::randomNumber(int min, int max) const {
  /* setup random device as seed and feed it to the mersenne twister engine */
  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<int> v;
  v.reserve(max);

  for (int i{min}; i <= max; i++) {
    v.push_back(i);
  }

  /* shake it */
  std::shuffle(v.begin(), v.end(), gen);

  return v.at(0);
}
} // namespace Lyli::Security