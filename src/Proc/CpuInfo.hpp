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
#ifndef COM_SRC_PROC_CPUINFO_HPP
#define COM_SRC_PROC_CPUINFO_HPP

#include <map>
#include <string>
#include <vector>
namespace Com::Proc {

class CpuInfo
{
public:
	struct Logical {
		int processor = 0;
		int core_id = 0;
		int apicid = 0;
		float mhz = 0.0f;
	};
	struct Physical {
		std::vector<Logical> logicals;
		std::map<std::string, std::string> others;
	};

	bool load();

	/// <id, physical>
	std::map<int, Physical> const &data() const;

	size_t logical_count() const;

	size_t logical_count(int physical) const;

	std::vector<float> cpu_mhz() const;

private:
	std::map<int, Physical> _physicals;
};

}
#endif
