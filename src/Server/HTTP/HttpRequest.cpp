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
#include <format>
#include <string>
#include <string_view>

namespace Lyli::Server::HTTP {
HttpRequest::HttpRequest(std::string_view data) : valid(this->parse(data)) {}
HttpRequest::~HttpRequest() = default;

RequestType HttpRequest::getRequestType() const { return this->request_type; }
std::string_view HttpRequest::getPath() const { return this->path; }
std::string_view HttpRequest::getHeaderValue(const std::string &key) const {
  return this->header.at(key);
}
std::string_view HttpRequest::getData() const { return this->data; }
bool HttpRequest::isValid() const { return this->valid; }

std::string_view HttpRequest::requestTypeToString(RequestType type) {
  switch (type) {
    using enum Lyli::Server::HTTP::RequestType;
  case GET:
    return "GET";
  case POST:
    return "POST";
  case PATCH:
    return "PATCH";
  case DELETE:
    return "DELETE";
  default:
    return "";
  }
}

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
      this->checkRequestType(std::string_view(data.cbegin(), ptr1));

  if (this->request_type == RequestType::NONE) {
    return false;
  }

  Utils::Logger::getInstance().debug(std::format(
      "RequType: {}", static_cast<std::size_t>(this->request_type)));

  /* Path */
  const char *ptr2 = Utils::StringUtils::ptrToNext(ptr1 + 1, ' ');
  if (ptr2 == nullptr)
    return false;

  this->path = std::string(ptr1 + 1, ptr2);

  return true;
}

std::pair<bool, const char *> HttpRequest::headerParse(std::string_view data) {
  const char *sptr = Utils::StringUtils::ptrToNext(data, '\n') + 1;
  Utils::Logger::getInstance().debug(sptr);
  if (sptr == nullptr)
    return std::pair(false, nullptr);

  std::string_view start{sptr};

  std::size_t nbeg{0};
  std::string name_buffer;

  for (std::size_t i{0}; i < start.size(); i++) {

    /* parse header name */
    if (start.at(i) == ':') {
      name_buffer = std::string(start.data() + nbeg, start.data() + i);
      Utils::Logger::getInstance().debug(
          std::format("Parsed Header Name: {}", name_buffer));
    }

    /* parse value for name */
    if (start.at(i) == ' ') {
      std::size_t vbeg{++i};
      while (start.at(i) != '\n')
        i++;

      this->header[name_buffer] =
          std::string_view(start.data() + vbeg, start.data() + i);
      Utils::Logger::getInstance().debug(
          std::format("Parsed value: {}", this->header.at(name_buffer)));
    }

    if (start.at(i) == '\n') {
      /* data found */
      if (start.size() > i + 2 && start.at(i + 1) == '\r' &&
          start.at(i + 2) == '\n') {
        Utils::Logger::getInstance().debug("Datablock found");
        return std::pair(true, &start.at(i + 2));
      }

      nbeg = i + 1;
    }
  }

  Utils::Logger::getInstance().debug("Return");
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
  using enum Lyli::Server::HTTP::RequestType;
  if (data == "GET")
    return GET;

  if (data == "POST")
    return POST;

  if (data == "PATCH")
    return PATCH;

  if (data == "DELETE")
    return DELETE;

  return NONE;
}

} // namespace Lyli::Server::HTTP