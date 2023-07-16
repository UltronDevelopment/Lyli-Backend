// This file is part of the Lyli Backend
// File:    Security/SpamAgent.hpp
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

#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <unordered_map>

namespace Lyli::Security {
using SpamAgentEntry = struct SpamAgentEntry {
  std::size_t valid_until{};
  std::size_t connection_count;
  bool blocked{false};
};

/* tracks ip connects and blocks them, if they send to many request in a given
 * time */
class SpamAgent {
public:
  SpamAgent();
  ~SpamAgent() = default;

  bool writeEntry(const std::string &ip);

  bool isRouteGuarded(std::string_view route) const;

  bool isIpBlocked(const std::string &ip);

  void guardRoute(std::string_view route);

  void start();

  /* blocks until all SpamAgent threads are finished */
  void stop();

private:
  /* This function will be designed for an extra thread. The thread is reading
   * the entries and blocks an ip, if it is spamming */
  void checkLoop();

  /* custom transparent heterogeneous hasher */
  struct StringHash {
    /* enables heterogeneous lookup */
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const {
      std::hash<std::string_view> hasher;
      return hasher(sv);
    }
  };

  /* in seconds */
  std::size_t time_valid;

  /* in seconds */
  std::size_t threshold;

  /* in seconds */
  std::size_t block_time;

  std::unordered_map<std::string, SpamAgentEntry, StringHash, std::equal_to<>>
      map;

  std::set<std::string_view, std::less<>> guarded_routes;

  std::mutex lock;
  std::condition_variable condition_var;

  std::unique_ptr<std::thread> spam_agent_viewer;

  bool stopping = false;
};
} // namespace Lyli::Security