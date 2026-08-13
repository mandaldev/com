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
#include "CpuInfo.hpp"
#include "load.hpp"
#include <charconv>

namespace Com::Proc {

bool CpuInfo::load()
{
	auto raw_data = load_header_value_style("/proc/cpuinfo", ":");
	if (raw_data.empty()) {
		return false;
	}
	raw_data.emplace_back("processor", "0");

	int pid = -1;
	Physical p;
	for (auto const &[h, v]: raw_data) {
		if (h == "processor") {
			if (pid != -1) {
				auto it = _physicals.find(pid);
				if (it != _physicals.end()) {
					it->second.logicals.push_back(p.logicals.back());
				} else {
					_physicals[pid] = p;
				}
			}
			p = {};
			auto &l = p.logicals.emplace_back();
			std::from_chars(&v.front(), &v.back() + 1, l.processor);
		} else if (h == "physical id") {
			std::from_chars(&v.front(), &v.back() + 1, pid);
		} else if (h == "core id") {
			std::from_chars(&v.front(), &v.back() + 1, p.logicals.back().core_id);
		} else if (h == "apicid") {
			std::from_chars(&v.front(), &v.back() + 1, p.logicals.back().apicid);
		} else if (h == "cpu MHz") {
			std::from_chars(&v.front(), &v.back() + 1, p.logicals.back().mhz);
		} else {
			p.others[h] = v;
		}
	}
	return true;
}

std::map<int, CpuInfo::Physical> const &CpuInfo::data() const
{
	return _physicals;
}

size_t CpuInfo::logical_count() const
{
	size_t n = 0;
	for (auto &[id, p] : _physicals) {
		n += p.logicals.size();
	}
	return n;
}

size_t CpuInfo::logical_count(int physical) const
{
	if (auto it = _physicals.find(physical); it != _physicals.end()) {
		return it->second.logicals.size();
	}
	return 0;
}

std::vector<float> CpuInfo::cpu_mhz() const
{
	std::vector<float> ret;
	for (auto &[id, p] : _physicals) {
		for (auto &l : p.logicals) {
			ret.push_back(l.mhz);
		}
	}
	return ret;
}

}
