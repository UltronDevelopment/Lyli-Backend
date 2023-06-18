// This file is part of the Lyli Backend
// File:    Signals/SignalHandler.hpp
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

#pragma once

namespace Lyli::SignalHandler {
/* cleanup and termintate programm */
void gracefullShutdown(int sig);
} // namespace Lyli::SignalHandler