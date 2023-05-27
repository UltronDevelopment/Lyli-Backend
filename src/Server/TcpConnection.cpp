// This file is part of the Lyli Backend
// File:    Server/TcpConnection.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    26 May 2032
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

#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind.hpp>

#include <Server/TcpConnection.hpp>
#include <Utils/Logger.hpp>

#include <format>

namespace Lyli::Server {
TcpConnection::TcpConnection(boost::asio::io_context &io) : _socket(io) {}
TcpConnection::~TcpConnection() = default;

boost::asio::ip::tcp::socket &TcpConnection::getSocket() {
  return this->_socket;
}

void TcpConnection::write() {

  this->_data = "lol";

  boost::asio::async_write(
      this->_socket, boost::asio::buffer(this->_data),
      boost::bind(&TcpConnection::handle_write, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void TcpConnection::handle_write(const boost::system::error_code &code,
                                 size_t bytes) const {
  const auto &logger = Utils::Logger::getInstance();

  Utils::Logger::getInstance().debug(
      std::format("TCP Server wrote {} bytes", bytes));

  if (!code) {
    logger.error(std::format("TCP Server Error: {}", code.message()));
    return;
  }
}
} // namespace Lyli::Server
