// File:    Server/TcpServer.cpp
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

#include <boost/bind.hpp>

#include <Server/TcpConnection.hpp>
#include <Server/TcpServer.hpp>
#include <Utils/Logger.hpp>

#include <memory>

namespace Lyli::Server {
TcpServer::TcpServer()
    : io(boost::asio::io_context{5}),
      _acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                   port)) {
  /* start listener */
  Utils::Logger::getInstance().trace(
      std::format("TcpServer init listener on port {}", TcpServer::port));
  this->listen();

  Utils::Logger::getInstance().trace("TcpServer io.run");
  this->io.run();
}

TcpServer::~TcpServer() = default;

void TcpServer::listen() {
  auto connection = TcpConnection::create(this->io);

  this->_acceptor.async_accept(connection->getSocket(),
                               boost::bind(&TcpServer::handle_accept, this,
                                           connection,
                                           boost::asio::placeholders::error));
}

void TcpServer::handle_accept(std::shared_ptr<TcpConnection> &connection,
                              const boost::system::error_code &error) {
  Utils::Logger::getInstance().trace(std::format(
      "Incoming connection from {}",
      connection->getSocket().remote_endpoint().address().to_string()));

  if (!error) {
    Utils::Logger::getInstance().trace("handling connection...");
    connection->write();
  }

  this->listen();
}
} // namespace Lyli::Server
