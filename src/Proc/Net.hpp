// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2026  Mandal Dev
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef COM_SRC_PROC_NET_HPP
#define COM_SRC_PROC_NET_HPP
#include <cstddef>
#include <string>
#include <vector>

namespace Com::Proc::Net {

struct Dev {
	std::string name;
	struct {
		size_t bytes = 0, packets = 0, errs = 0, drop = 0, fifo = 0, frame = 0, compressed = 0, multicast = 0;
	} receive;
	struct {
		size_t bytes = 0, packets = 0, errs = 0, drop = 0, fifo = 0, colls = 0, carrier = 0, compressed = 0;
	} transmit;
};

std::vector<Dev> dev();

}
#endif
