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
#include "Stat.hpp"
#include "load.hpp"
#include <chrono>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <vector>

namespace Com::Proc {

std::chrono::milliseconds Stat::CpuTime::total() const
{
	return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
}

float Stat::CpuTime::usage_percent(CpuTime const &old, float *system) const
{
	auto d = total() - old.total();
	if (system) {
		*system = 100.0 * ((double)(this->system - old.system).count() / (double)d.count());
	}
	return 100.0 * (1.0 - (double)(idle - old.idle).count() / (double)d.count());
}

bool Stat::load()
{
	auto raw_data = load_header_value_style("/proc/stat", " ");
	if (raw_data.empty()) {
		return false;
	}

	auto const hz = sysconf(_SC_CLK_TCK);

	for (auto const &[header, value] : raw_data) {
		auto iss = std::istringstream(value);
		auto ii = std::istream_iterator<long>(iss);
		std::vector<long> ints(ii, std::istream_iterator<long>());
		if (header.substr(0, 3) == "cpu") {
			if (ints.size() >= 10) {
				CpuTime *cpu = &_cpu_total;
				if (header != "cpu") {
					cpu = &_cpu.emplace_back();
				}
				cpu->user = std::chrono::milliseconds(ints[0] * 1000 / hz);
				cpu->nice = std::chrono::milliseconds(ints[1] * 1000 / hz);
				cpu->system = std::chrono::milliseconds(ints[2] * 1000 / hz);
				cpu->idle = std::chrono::milliseconds(ints[3] * 1000 / hz);
				cpu->iowait = std::chrono::milliseconds(ints[4] * 1000 / hz);
				cpu->irq = std::chrono::milliseconds(ints[5] * 1000 / hz);
				cpu->softirq = std::chrono::milliseconds(ints[6] * 1000 / hz);
				cpu->steal = std::chrono::milliseconds(ints[7] * 1000 / hz);
				cpu->guest = std::chrono::milliseconds(ints[8] * 1000 / hz);
				cpu->guest_nice = std::chrono::milliseconds(ints[9] * 1000 / hz);
			}
			continue;
		}
		_others[header] = ints;
	}

	return true;
}

Stat::CpuTime const &Stat::get_cpu_total_time() const
{
	return _cpu_total;
}

std::vector<Stat::CpuTime> const &Stat::get_cpu_time() const
{
	return _cpu;
}

std::map<std::string, std::vector<long>> Stat::get_others() const
{
	return _others;
}

}
