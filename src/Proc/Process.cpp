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
#include "Process.hpp"
#include "../file.hpp"
#include "../string.hpp"
#include <charconv>
#include <filesystem>
#include <sstream>
#include <string>
#include <unistd.h>

namespace Com::Proc::Process {

Stat stat(std::string const &pid)
{
	auto opt = load_file("/proc/" + pid + "/stat");
	if (!opt) {
		return {};
	}
	auto const &line = opt.value();
	auto items = Com::split(std::string_view(line), "(");
	if (items.size() < 2) {
		return {};
	}
	auto pid_str = items[0];
	items = Com::split(items[1], ")");
	if (items.size() < 2) {
		return {};
	}
	auto comm_str = items[0];
	Stat ret;
	{
		std::istringstream iss(std::string{pid_str});
		iss >> ret.pid;
	}
	ret.comm = comm_str;
	std::istringstream iss(std::string{ items[1] });
	iss
	>> ret.state
	>> ret.ppid
	>> ret.pgrp
	>> ret.session
	>> ret.tty_nr
	>> ret.tpgid
	>> ret.flags
	>> ret.minflt
	>> ret.cminflt
	>> ret.majflt
	>> ret.cmajflt
	>> ret.utime
	>> ret.stime
	>> ret.cutime
	>> ret.cstime
	>> ret.priority
	>> ret.nice
	>> ret.num_threads
	>> ret.itrealvalue
	>> ret.starttime
	>> ret.vsize
	>> ret.rss
	>> ret.rsslim
	>> ret.startcode
	>> ret.endcode
	>> ret.startstack
	>> ret.kstkesp
	>> ret.kstkeip
	>> ret.signal
	>> ret.blocked
	>> ret.sigignore
	>> ret.sigcatch
	>> ret.wchan
	>> ret.nswap
	>> ret.cnswap
	>> ret.exit_signal
	>> ret.processor
	>> ret.rt_priority
	>> ret.policy
	>> ret.delayacct_blkio_ticks
	>> ret.guest_time
	>> ret.cguest_time
	>> ret.start_data
	>> ret.end_data
	>> ret.start_brk
	>> ret.arg_start
	>> ret.arg_end
	>> ret.env_start
	>> ret.env_end
	>> ret.exit_code
	;
	return ret;
}

Stat stat(int pid)
{
	char buf[64] = "";
	std::to_chars(buf, buf + sizeof(buf), pid);
	return stat(buf);
}

Statm statm(std::string const &pid)
{
	auto opt = load_file("/proc/" + pid + "/statm");
	if (!opt) {
		return {};
	}
	Statm ret;
	std::istringstream iss(opt.value());
	iss
	>> ret.size
	>> ret.resident
	>> ret.shared
	>> ret.text
	>> ret.lib
	>> ret.data
	>> ret.dt
	;
	return ret;
}

Statm statm(int pid)
{
	char buf[64] = "";
	std::to_chars(buf, buf + sizeof(buf), pid);
	return statm(buf);
}

unsigned long page_size()
{
	static unsigned long ret = sysconf(_SC_PAGESIZE);
	return ret;
}

Tree tree()
{
	Tree ret;
	for (auto const &de : std::filesystem::directory_iterator("/proc")) {
		auto name = de.path().stem().string();
		if (name.find_first_not_of("0123456789") == name.npos) {
			auto s = stat(name);
			auto sm = statm(name);
			if (s.pid == 0) {
				continue;
			}
			ret.stats[s.pid] = std::move(s);
			ret.statms[s.pid] = std::move(sm);
			ret.children[s.ppid].push_back(s.pid);
		}
	}

	return ret;
}

}
