// File:    main.cpp
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

#include <boost/asio/io_context.hpp>

#include <Server/TcpServer.hpp>
#include <Utils/Logger.hpp>

int main() {
  const auto &u = Lyli::Utils::Logger::getInstance();

  /* gets only printed on debug builds */
  u.debug("!!! DEBUG BUILD !!!");

  u.trace("Lyli Backend starting...");
  Lyli::Server::TcpServer server{};
  server.listen();

  return 0;
}
