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
#include "devices.hpp"
#include "load.hpp"
#include <charconv>
#include <string>

namespace Com::Proc {

Devices devices()
{
	Devices ret;
	auto cur = &ret.character;
	auto raw_data = Proc::load_header_value_style("/proc/devices", " ");
	for (auto const &[h, v]: raw_data) {
		if (v == "devices:") {
			if (h == "Character") {
				cur = &ret.character;
			} else if (h == "Block") {
				cur = &ret.block;
			}
			continue;
		}
		unsigned major = 0;
		std::from_chars(&h.front(), &h.back() + 1, major);
		if (major) {
			(*cur)[major] = v;
		}
	}
	return ret;
}

}
