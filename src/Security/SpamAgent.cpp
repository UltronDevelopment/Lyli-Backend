// This file is part of the Lyli Backend
// File:    Security/SpamAgent.cpp
// Author:  Mina <mina@upndevelopment.de>
// Date:    16 July 2023
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

#include <Security/SpamAgent.hpp>
#include <Utils/FormattedTime.hpp>
#include <Utils/Logger.hpp>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

namespace Lyli::Security {
SpamAgent::SpamAgent() {
  const char *env_threshold = std::getenv("SA_THRESHOLD");
  env_threshold == nullptr
      ? SpamAgent::threshold = 10
      : SpamAgent::threshold = strtoul(env_threshold, nullptr, 10);

  const char *env_entrytime = std::getenv("SA_ENTRYTIME");
  env_entrytime == nullptr
      ? SpamAgent::time_valid = 10
      : SpamAgent::time_valid = strtoul(env_entrytime, nullptr, 10);

  const char *env_blocktime = std::getenv("SA_BLOCKTIME");
  env_blocktime == nullptr
      ? SpamAgent::block_time = 3600
      : SpamAgent::block_time = strtoul(env_blocktime, nullptr, 10);

  Utils::Logger::getInstance().trace("SpamAgent setup complete");
}

bool SpamAgent::writeEntry(const std::string &ip) {
  /* lock write section */
  std::scoped_lock m{lock};

  /* if another thread already blocked this ip, return false */
  if (this->map.contains(ip) && this->map.at(ip).blocked) {
    return false;
  }

  if (!this->map.contains(ip) && !this->map.try_emplace(ip).second)
    return false;

  this->map.at(ip).connection_count += 1;

  /* if the entry is new, set how long its valid */
  if (this->map.at(ip).valid_until == 0) {
    this->map.at(ip).valid_until =
        Utils::FormattedTime::UNIX() + this->time_valid;
  }
  Utils::Logger::getInstance().debug(
      std::to_string(this->map.at(ip).valid_until));

  this->condition_var.notify_one();
  return true;
}

bool SpamAgent::isRouteGuarded(std::string_view route) const {
  return this->guarded_routes.contains(route);
}

bool SpamAgent::isIpBlocked(const std::string &ip) {
  std::scoped_lock m{lock};
  if (!this->map.contains(ip))
    return false;

  return this->map.at(ip).blocked;
}

void SpamAgent::guardRoute(std::string_view route) {
  this->guarded_routes.insert(route);
}

void SpamAgent::start() {
  if (this->stopping) {
    return;
  }

  this->stopping = false;

  Utils::Logger::getInstance().debug("Start second thread");
  this->spam_agent_viewer =
      std::make_unique<std::thread>(&SpamAgent::checkLoop, this);
}

void SpamAgent::stop() {
  if (!this->stopping) {
    Utils::Logger::getInstance().debug("SpamAgent: stop");
    this->stopping = true;
  }

  this->condition_var.notify_one();
  this->spam_agent_viewer->join();
}

void SpamAgent::checkLoop() {
  while (!stopping) {
    std::unique_lock m{this->lock};
    this->condition_var.wait(
        m, [this]() { return !this->map.empty() || this->stopping; });

    if (stopping)
      break;

    const auto time = Utils::FormattedTime::UNIX();
    if (auto it =
            std::find_if(this->map.cbegin(), this->map.cend(),
                         [&](const std::pair<std::string, SpamAgentEntry> &e) {
                           return e.second.valid_until < time;
                         });
        it != this->map.cend())
      this->map.erase(it);

    for (auto &[key, entry] : this->map) {
      if (!entry.blocked && entry.connection_count >= this->threshold) {
        Utils::Logger::getInstance().trace("SpamAgent: block " + key);
        entry.blocked = true;
        entry.valid_until += this->block_time;
      }
    }

    m.unlock();
    condition_var.notify_one();
  }
  Utils::Logger::getInstance().debug("SpamAgent: finished thread");
}
} // namespace Lyli::Security