// This file is part of the Lyli Backend
// File:    main.cpp
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

#include <boost/asio/io_context.hpp>

#include <API/Router.hpp>
#include <DB/Client.hpp>
#include <Dotenv/Loader.hpp>
#include <Server/TcpServer.hpp>
#include <Utils/Logger.hpp>

#include <cstdlib>
#include <string_view>

const Lyli::Utils::Logger &logger = Lyli::Utils::Logger::getInstance();

static inline void loadEnv() {
  /* load variables from the .env file */
  logger.trace("Loading .env file");
  Lyli::Dotenv::Loader loader{};
  loader.load(".env");
}

static inline void startDatabase() {
  logger.trace("Start DB Driver...");

  auto &client = Lyli::DB::Client::getInstance();
  const char *srv = getenv("MONGO_SRV");

  if (srv == nullptr) {
    logger.error("No SRV");
    exit(1);
  }

  /* init db client */
  client.create(getenv("APPLICATION_NAME"), srv);
  if (!client.isValid())
    exit(1);

  /* open a database and a collection */
  auto db{client.openDB(getenv("DB_NAME"))};
  db->openCollection(getenv("USER_COLLECTION_NAME"));
}

static inline void startServer() {
  /* setup router */
  logger.trace("Setup Router...");
  Lyli::API::Router::getInstance().setup();

  /* start server */
  logger.trace("TCP Server starting...");
  Lyli::Server::TcpServer server{};
}

int main() {
  loadEnv();
  startDatabase();
  startServer();

  return 0;
}
