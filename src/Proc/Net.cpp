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
#include "Net.hpp"
#include "load.hpp"
#include <sstream>

namespace Com::Proc::Net {

std::vector<Dev> dev()
{
	auto raw_data = load_header_value_style("/proc/net/dev", ":");
	std::vector<Dev> ret;
	for (auto const &[h, v]: raw_data) {
		auto &dev = ret.emplace_back();
		dev.name = h;
		std::istringstream iss(v);
		iss
		>> dev.receive.bytes
		>> dev.receive.packets
		>> dev.receive.errs
		>> dev.receive.drop
		>> dev.receive.fifo
		>> dev.receive.frame
		>> dev.receive.compressed
		>> dev.receive.multicast
		>> dev.transmit.bytes
		>> dev.transmit.packets
		>> dev.transmit.errs
		>> dev.transmit.drop
		>> dev.transmit.fifo
		>> dev.transmit.colls
		>> dev.transmit.carrier
		>> dev.transmit.compressed
		;
	}
	return ret;
}

}
