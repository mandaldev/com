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
#include "DiskStats.hpp"
#include "../file.hpp"
#include <chrono>
#include <sstream>

namespace Com::Proc {

bool DiskStats::load(bool diff)
{
	auto first = _data.empty();
	auto old = std::move(_data);
	auto opt = load_file("/proc/diskstats");
	auto t = std::chrono::steady_clock::now();
	if (!opt) {
		return false;
	}

	std::istringstream iss(opt.value());
	while (iss) try {
		unsigned t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;
		Stat stat;
		iss >> stat.major >> stat.minor >> stat.name;
		if (stat.name.empty()) {
			break;
		}
		iss >> stat.read.completed
		>> stat.read.merged
		>> stat.read.sectors
		>> t1
		>> stat.write.completed
		>> stat.write.merged
		>> stat.write.sectors
		>> t2
		>> stat.in_progress
		>> t3
		>> t4
		>> stat.discard.completed
		>> stat.discard.merged
		>> stat.discard.sectors
		>> t5
		>> stat.flush
		>> t6
		;
		stat.read.spent = std::chrono::milliseconds(t1);
		stat.write.spent = std::chrono::milliseconds(t2);
		stat.io_spent = std::chrono::milliseconds(t3);
		stat.weighted_io_spent = std::chrono::milliseconds(t4);
		stat.discard.spent = std::chrono::milliseconds(t5);
		stat.flush_spent = std::chrono::milliseconds(t6);
		auto &tmp = _data[stat.name];
		tmp = std::move(stat);
	} catch (...) {
	}

	std::chrono::duration<double> d = t - _load_t;
	if (!first && diff && d.count() > 0) {
		_diff.clear();
		for (auto const &[name, stat] : _data) {
			auto it = old.find(name);
			if (it == old.end()) {
				continue;
			}
			auto &diff = _diff[name];
			diff.read_speed = (stat.read.sectors - it->second.read.sectors) * 512 / d.count();
			diff.write_speed = (stat.write.sectors - it->second.write.sectors) * 512 / d.count();
			auto io_spent_d = stat.weighted_io_spent - it->second.weighted_io_spent;
			diff.usage = 100 * (io_spent_d).count() / (1000 * d.count());
		}
	}

	_load_t = t;
	return true;
}

std::map<std::string, DiskStats::Stat> const &DiskStats::data() const
{
	return _data;
}

std::map<std::string, DiskStats::Diff> const &DiskStats::diff() const
{
	return _diff;
}

}
