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
#include "filesystems.hpp"
#include "load.hpp"

namespace Com::Proc {

std::set<std::string> filesystems(std::set<std::string> *nodev_filesystems)
{
	std::set<std::string> ret;
	if (nodev_filesystems) {
		nodev_filesystems->clear();
	}

	auto raw_data = Proc::load_values_style("/proc/filesystems", "\t");
	for (auto const &values: raw_data) {
		if (values.empty()) {
			continue;
		} else if (values.size() == 1) {
			ret.insert(values[0]);
		} else if (values[0] == "nodev" && nodev_filesystems) {
			nodev_filesystems->insert(values[1]);
		}
	}
	return ret;
}

}
