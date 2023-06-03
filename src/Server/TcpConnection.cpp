// This file is part of the Lyli Backend
// File:    Server/TcpConnection.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    26 May 2023
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

#include <boost/asio/completion_condition.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>

#include <API/Router.hpp>
#include <Server/HTTP/HttpRequest.hpp>
#include <Server/TcpConnection.hpp>
#include <Utils/Logger.hpp>

#include <format>
#include <memory>
#include <string_view>

namespace Lyli::Server {
TcpConnection::TcpConnection(boost::asio::io_context &io)
    : io(io), _socket(io) {}
TcpConnection::~TcpConnection() = default;

boost::asio::ip::tcp::socket &TcpConnection::getSocket() {
  return this->_socket;
}
void TcpConnection::write() {
  boost::asio::async_write(
      this->_socket, boost::asio::buffer("lol"),
      boost::bind(&TcpConnection::handle_write, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TcpConnection::read() {
  /* allocate 16KiB for incoming data */
  this->_rdbuf.resize(0x1000);

  this->_socket.async_read_some(
      boost::asio::buffer(this->_rdbuf),
      [con = this->shared_from_this()](const boost::system::error_code &code,
                                       size_t bytes) {
        const auto &adress =
            con->_socket.remote_endpoint().address().to_string();

        Utils::Logger::getInstance().debug(
            std::format("Read | {} Bytes", bytes));

        Utils::Logger::getInstance().debug(con->_rdbuf);

        /* Parse to HTTP Objects */
        Lyli::Server::HTTP::HttpRequest req{con->_rdbuf};
        if (!req.isValid()) {
          Utils::Logger::getInstance().error(
              std::format("Got invalid http request from {}", adress));
          return;
        }

        /* log RequestType and Path */
        Utils::Logger::getInstance().trace(std::format(
            "[{}] {} {}", adress,
            HTTP::HttpRequest::requestTypeToString(req.getRequestType()),
            req.getPath()));

        if (code.failed()) {
          Utils::Logger::getInstance().error(
              std::format("TCP Server Error: {}", code.message()));
          return;
        }
      });
}

void TcpConnection::handle_write(const boost::system::error_code &code,
                                 size_t bytes) const {
  const auto &logger = Utils::Logger::getInstance();

  Utils::Logger::getInstance().debug(
      std::format("TCP Server wrote {} bytes", bytes));

  if (code.failed()) {
    logger.error(std::format("TCP Server Error: {}", code.message()));
    return;
  }
}

} // namespace Lyli::Server
