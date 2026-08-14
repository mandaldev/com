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
#include "MemInfo.hpp"
#include "load.hpp"
#include "../string.hpp"
#include <charconv>
#include <string_view>

namespace Com::Proc {


bool MemInfo::load()
{
	static auto const UNIT = std::map<std::string_view, size_t>{
		{ "B", 1 },
		{ "kB", 1<<10 },
		{ "KB", 1<<10 },
		{ "mB", 1<<20 },
		{ "MB", 1<<20 },
		{ "gB", 1<<30 },
		{ "GB", 1<<30 },
	};
	*this = {};
	auto raw_data = load_header_value_style("/proc/meminfo", ":");
	if (raw_data.empty()) {
		return false;
	}
	for (auto const &[h, v] : raw_data) {
		auto ss = Com::split(std::string_view(v), " ");
		size_t value = 0;
		auto value_str = ss[0];
		std::from_chars(&value_str.front(), &value_str.back() + 1, value);
		if (ss.size() > 1) {
			auto it = UNIT.find(ss[1]);
			if (it != UNIT.end()) {
				value *= it->second;
			}
		}
		if (h == "MemTotal") {
			_total = value;
		} else if (h == "MemFree") {
			_free = value;
		} else if (h == "MemAvailable") {
			_available = value;
		} else if (h == "Cached") {
			_cached = value;
		} else if (h == "SwapTotal") {
			_swap_total = value;
		} else if (h == "SwapFree") {
			_swap_free = value;
		} else {
			_others[h] = value;
		}
	}
	return true;
}

size_t MemInfo::total() const
{
	return _total;
}

size_t MemInfo::free() const
{

	return _free;
}

size_t MemInfo::available() const
{
	return _available;
}

size_t MemInfo::cached() const
{
	return _cached;
}

size_t MemInfo::swap_total() const
{
	return _swap_total;
}

size_t MemInfo::swap_free() const
{
	return _swap_free;
}

std::map<std::string, size_t> const &MemInfo::others() const
{
	return _others;
}

}
