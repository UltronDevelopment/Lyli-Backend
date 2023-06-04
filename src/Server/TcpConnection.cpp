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
#include <Server/HTTP/HttpResponse.hpp>
#include <Server/TcpConnection.hpp>
#include <Utils/FormattedTime.hpp>
#include <Utils/Logger.hpp>

#include <memory>
#include <string_view>

namespace Lyli::Server {
TcpConnection::TcpConnection(boost::asio::io_context &io)
    : io(io), _socket(io) {}
TcpConnection::~TcpConnection() = default;

boost::asio::ip::tcp::socket &TcpConnection::getSocket() {
  return this->_socket;
}

void TcpConnection::write(std::string_view data) {
  Lyli::Server::HTTP::HttpResponse resp{};
  resp.setCode(Lyli::Server::HTTP::ResponseCode::OK);
  resp.setData(data);

  resp.setHeaderValue("date", Lyli::Utils::FormattedTime::HTTP());
  resp.setHeaderValue("server", "lyli-backend");
  resp.setHeaderValue("Content-Type", "application/json; charset=utf-8");
  resp.setHeaderValue("connection", "close");

  boost::asio::async_write(
      this->_socket, boost::asio::buffer(resp.toString()),
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
        TcpConnection::respond(con, code, bytes);
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

void TcpConnection::respond(std::shared_ptr<TcpConnection> con,
                            const boost::system::error_code &code,
                            size_t bytes) {
  const auto &adress = con->_socket.remote_endpoint().address().to_string();

  Utils::Logger::getInstance().debug(std::format("Read | {} Bytes", bytes));

  if (code.failed()) {
    Utils::Logger::getInstance().error(
        std::format("TCP Connection Error: {}", code.message()));
    return;
  }

  /* Parse to HTTP Object */
  auto req{HTTP::HttpRequest::create(con->_rdbuf)};
  if (req == nullptr) {
    Utils::Logger::getInstance().error(
        std::format("Got invalid http request from {}", adress));
    return;
  }

  /* log RequestType and Path */
  Utils::Logger::getInstance().trace(
      std::format("[{}] {} {}", adress,
                  HTTP::HttpRequest::requestTypeToString(req->getRequestType()),
                  req->getPath()));

  /* get handler function as function ptr */
  auto *handler = API::Router::getInstance().route(req->getPath());

  /* it should be impossible that handler is null here, but just in case */
  if (handler == nullptr) {
    Utils::Logger::getInstance().error(
        "Found no handler (that is fucking bad and should be fixed RIGHT "
        "NOW)");
    /* reset connection, cause i wrote garbage */
    return;
  }

  Utils::Logger::getInstance().debug(handler(req));
  con->write(R"({"id": 0, "isHurensohn": true})");
}
} // namespace Lyli::Server
