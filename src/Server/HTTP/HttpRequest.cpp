// This file is part of the Lyli Backend
// File:    Server/HTTP/HttpRequest.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    28 May 2023
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

#include <Server/HTTP/HttpRequest.hpp>
#include <Utils/Logger.hpp>
#include <Utils/StringUtils.hpp>

#include <memory>
#include <string>
#include <string_view>

namespace Lyli::Server::HTTP {
HttpRequest::HttpRequest(std::string_view data) {
  this->valid = this->parse(data);
}

HttpRequest::~HttpRequest() = default;

RequestType HttpRequest::getRequestType() const { return this->request_type; }
std::string_view HttpRequest::getPath() const { return this->path; }

bool HttpRequest::parse(std::string_view data) {
  if (!this->firstParse(data))
    return false;

  Utils::Logger::getInstance().debug("HTTP: Parse Header");

  auto [passed, data_ptr] = this->headerParse(data);
  if (!passed || data_ptr == nullptr)
    return false;

  if (!this->dataParse(data_ptr + 1))
    return false;

  return true;
}

bool HttpRequest::firstParse(std::string_view data) {
  /* Hot garbage */

  if (data.empty())
    return false;

  /* Request Type */
  const char *ptr1 = Utils::StringUtils::ptrToNext(data, ' ');
  if (ptr1 == nullptr)
    return false;

  this->request_type =
      this->checkRequestType(std::string(data.cbegin(), ptr1));

  if (this->request_type == RequestType::NONE) {
    return false;
  }

  /* Path */
  const char *ptr2 = Utils::StringUtils::ptrToNext(ptr1 + 1, ' ');
  if (ptr2 == nullptr)
    return false;

  this->path = std::string(ptr1 + 1, ptr2);

  return true;
}

std::pair<bool, const char *> HttpRequest::headerParse(std::string_view data) {
  const char *sptr = Utils::StringUtils::ptrToNext(data, '\n') + 1;
  if (sptr == nullptr)
    return std::pair(false, nullptr);

  std::string_view start{sptr};

  std::size_t nbeg{0};
  std::string name_buffer;

  for (std::size_t i{0}; i < start.size(); i++) {

    /* parse header name */
    if (start.at(i) == ':') {
      name_buffer = std::string(start.data() + nbeg, start.data() + i);
    }

    /* parse value for name */
    if (start.at(i) == ' ') {
      std::size_t vbeg{++i};
      while (start.at(i) != '\n')
        i++;

      this->header[name_buffer] =
          std::string(start.data() + vbeg, start.data() + i);
    }

    if (start.at(i) == '\n') {
      /* data found */
      if (start.size() > i + 2 && start.at(i + 1) == '\r' &&
          start.at(i + 2) == '\n') {
        return std::pair(true, &start.at(i + 2));
      }

      nbeg = i + 1;
    }
  }

  return std::pair(true, nullptr);
}

bool HttpRequest::dataParse(std::string_view data) {
  if (!this->header.contains("Content-Type") ||
      !this->header.contains("Content-Length"))
    return true;

  this->data = data;
  if (this->data.size() != std::stoi(this->header.at("Content-Length")))
    return false;

  Utils::Logger::getInstance().debug("Got data successful");
  return true;
}

RequestType HttpRequest::checkRequestType(std::string_view data) const {
  if (data == "GET")
    return RequestType::GET;

  if (data == "POST")
    return RequestType::POST;

  if (data == "PATCH")
    return RequestType::PATCH;

  if (data == "DELETE")
    return RequestType::DELETE;

  return RequestType::NONE;
}

std::shared_ptr<HttpRequest> HttpRequest::create(std::string_view data) {
  auto http = std::make_shared<HttpRequest>(data);
  if (!http->isValid())
    return nullptr;

  return http;
}

} // namespace Lyli::Server::HTTP