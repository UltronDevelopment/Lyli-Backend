// This file is part of the Lyli Backend
// File:    Server/TcpConnection.hpp
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

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory>

namespace Lyli::Server {
class TcpConnection {
public:
  explicit TcpConnection(boost::asio::io_context &io);
  ~TcpConnection();

  static std::shared_ptr<TcpConnection> create(boost::asio::io_context &io) {
    return std::make_shared<TcpConnection>(io);
  }

  boost::asio::ip::tcp::socket &getSocket();

  void write();

private:
  void handle_write(const boost::system::error_code &code, size_t bytes);

  boost::asio::ip::tcp::socket _socket;
  std::string _data;
};
} // namespace Lyli::Server
