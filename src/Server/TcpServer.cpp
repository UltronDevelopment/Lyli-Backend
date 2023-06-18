// This file is part of the Lyli Backend
// File:    Server/TcpServer.cpp
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

#include <boost/bind/bind.hpp>

#include <Server/TcpConnection.hpp>
#include <Server/TcpServer.hpp>
#include <Utils/Logger.hpp>

#include <memory>

namespace Lyli::Server {
TcpServer::TcpServer()
    : io(boost::asio::io_context{10}),
      _acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                   port)) {}

TcpServer::~TcpServer() = default;

TcpServer &TcpServer::getInstance() {
  static TcpServer instance;
  return instance;
}

void TcpServer::start() {
  auto &server{TcpServer::getInstance()};

  /* start listener */
  Utils::Logger::getInstance().trace("TcpServer init listener");
  server.listen();

  Utils::Logger::getInstance().trace("TcpServer io.run()");
  server.io.run();
}

void TcpServer::listen() {
  auto &server{TcpServer::getInstance()};

  auto connection = TcpConnection::create(this->io);

  server._acceptor.async_accept(connection->getSocket(),
                                boost::bind(&TcpServer::handle_accept, this,
                                            connection,
                                            boost::asio::placeholders::error));
}

void TcpServer::stop() {
  auto &server{TcpServer::getInstance()};

  Utils::Logger::getInstance().trace("TCP Server STOP");
  server.io.stop();
}

void TcpServer::handle_accept(std::shared_ptr<TcpConnection> &connection,
                              const boost::system::error_code &error) {
  Utils::Logger::getInstance().debug(
      "Incoming connection from " +
      connection->getSocket().remote_endpoint().address().to_string());

  if (error.failed()) {
    Utils::Logger::getInstance().error("TCP Server: " + error.message());
    return;
  }

  Utils::Logger::getInstance().debug("handling connection...");
  connection->read();

  this->listen();
}
} // namespace Lyli::Server
