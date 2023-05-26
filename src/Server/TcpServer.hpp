// File:    Server/TcpServer.hpp
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
// (c) UPN
//

#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <Server/TcpConnection.hpp>

namespace Lyli::Server {
class TcpServer {
public:
  TcpServer();
  ~TcpServer();

  void listen();

private:
  void handle_accept(std::shared_ptr<TcpConnection> &connection,
                     const boost::system::error_code &error);

  static constexpr std::uint16_t port = 1870;
  boost::asio::io_context io;
  boost::asio::ip::tcp::acceptor _acceptor;
};
} // namespace Lyli::Server
