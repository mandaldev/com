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
#ifndef COM_SRC_PROC_STAT_HPP
#define COM_SRC_PROC_STAT_HPP
#include <chrono>
#include <vector>
#include <map>
#include <string>

namespace Com::Proc {

class Stat
{
public:
	struct CpuTime {
		std::chrono::milliseconds user, nice, system, idle,
			iowait, irq, softirq,
			steal, guest, guest_nice;

		std::chrono::milliseconds total() const;

		float usage_percent(CpuTime const &old, float *system = nullptr) const;
	};

	/// 从 `/proc/stat` 读取最新数据
	bool load();

	CpuTime const &get_cpu_total_time() const;

	std::vector<CpuTime> const &get_cpu_time() const;

	std::map<std::string, std::vector<long>> get_others() const;

private:
	CpuTime _cpu_total;
	std::vector<CpuTime> _cpu;
	std::map<std::string, std::vector<long>> _others;
};

}
#endif
