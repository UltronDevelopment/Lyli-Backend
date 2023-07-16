// This file is part of the Lyli Backend
// File:    Signals/SignalHandler.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    18 June 2023
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

#include <DB/Client.hpp>
#include <Server/TcpServer.hpp>
#include <Session.hpp>
#include <Signals/SignalHandler.hpp>
#include <Utils/Logger.hpp>

#include <cstdio>
#include <string>

namespace Lyli::SignalHandler {
void gracefullShutdown(int sig) {
  std::putc(10, stdout);
  Utils::Logger::getInstance().trace("Handling Signal " + std::to_string(sig));

  /* Stop TCP Server */
  Server::TcpServer::stop();

  /* stop the spam agent */
  Session::getInstance().getSpamAgent()->stop();

  /* close database connection */
  Session::getInstance().getDatabaseClient().close();
}
} // namespace Lyli::SignalHandler