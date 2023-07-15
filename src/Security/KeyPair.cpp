// This file is part of the Lyli Backend
// File:    Security/KeyPair.cpp
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

#include <crypto++/cryptlib.h>
#include <crypto++/rsa.h>
#include <crypto++/secblock.h>

#include <Security/KeyPair.hpp>
#include <Utils/Logger.hpp>
#include <Utils/StringUtils.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Lyli::Security {
KeyPair::KeyPair() {
  /* generate keypair */
  CryptoPP::InvertibleRSAFunction params;
  params.GenerateRandomWithKeySize(rng, 2048);

  this->privat_key = std::make_shared<CryptoPP::RSA::PrivateKey>(params);
  this->public_key = std::make_shared<CryptoPP::RSA::PublicKey>(params);

  Utils::Logger::getInstance().trace("Generated a new RSA KeyPair");
}

std::string KeyPair::encrypt(std::string_view data) {
  CryptoPP::RSAES_OAEP_SHA_Encryptor e(*this->public_key);

  std::size_t ecl = e.CiphertextLength(data.size());
  if (ecl == 0)
    return "";

  CryptoPP::SecByteBlock cipher(ecl);

  e.Encrypt(this->rng, reinterpret_cast<const std::uint8_t *>(data.data()),
            data.size(), cipher);

  return Utils::StringUtils::bytesToString(std::vector<std::uint8_t>(
      cipher.data(), cipher.data() + cipher.SizeInBytes()));
}

std::string KeyPair::decrypt(const std::vector<std::uint8_t> &data) {
  CryptoPP::RSAES_OAEP_SHA_Decryptor d(*this->privat_key);

  std::size_t dpl = d.MaxPlaintextLength(data.size());

  if (dpl == 0)
    return "";
  std::string recovered;
  recovered.resize(dpl);

  CryptoPP::DecodingResult res{};
  try {
    /* decrypt */
    res = d.Decrypt(this->rng, data.data(), data.size(),
                    reinterpret_cast<std::uint8_t *>(recovered.data()));
  }

  catch (const CryptoPP::Exception &e) {
    Utils::Logger::getInstance().error(e.GetWhat());
    return "";
  }

  /* sanity check */
  if (!res.isValidCoding) {
    Utils::Logger::getInstance().error(
        "Decrypted result is not a valid coding");
    return "";
  }

  return recovered;
}
} // namespace Lyli::Security