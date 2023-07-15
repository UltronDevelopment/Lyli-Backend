// This file is part of the Lyli Backend
// File:    Security/KeyPair.hpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    15 July 2023
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

#include <crypto++/osrng.h>
#include <crypto++/rsa.h>

#include <memory>

namespace Lyli::Security {
/* public private keypair*/
class KeyPair {
public:
  KeyPair();
  ~KeyPair() = default;

  std::string encrypt(std::string_view data);

  std::string decrypt(const std::vector<std::uint8_t> &data);

private:
  CryptoPP::AutoSeededRandomPool rng;
  std::shared_ptr<CryptoPP::RSA::PrivateKey> privat_key;
  std::shared_ptr<CryptoPP::RSA::PublicKey> public_key;
};
} // namespace Lyli::Security