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
#ifndef COM_SRC_PROC_MEMINFO_HPP
#define COM_SRC_PROC_MEMINFO_HPP
#include <cstddef>
#include <map>
#include <string>

namespace Com::Proc {

class MemInfo
{
public:
	bool load();

	size_t total() const;

	size_t free() const;

	size_t available() const;

	size_t cached() const;

	size_t swap_total() const;

	size_t swap_free() const;

	std::map<std::string, size_t> const &others() const;

private:
	size_t _total = 0;
	size_t _free = 0;
	size_t _available = 0;
	size_t _cached = 0;
	size_t _swap_total = 0;
	size_t _swap_free = 0;
	std::map<std::string, size_t> _others;
};

}
#endif
