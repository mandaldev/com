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
#ifndef COM_SRC_PROC_DISKSTATS_HPP
#define COM_SRC_PROC_DISKSTATS_HPP

#include <chrono>
#include <map>
#include <string>
namespace Com::Proc {

class DiskStats
{
public:
	struct Stat {
		int major = 0, minor = 0;
		std::string name;
		struct {
			unsigned long completed = 0;
			unsigned long merged = 0;
			/// 512 bytes per sector
			unsigned long sectors = 0;
			std::chrono::milliseconds spent;
		} read, write, discard;
		unsigned in_progress = 0;
		std::chrono::milliseconds io_spent;
		std::chrono::milliseconds weighted_io_spent;
		unsigned long flush = 0;
		std::chrono::milliseconds flush_spent;
	};
	struct Diff {
		/// 字节每秒
		double read_speed = 0, write_speed = 0;
		/// 利用率
		float usage = 0;
	};

	bool load(bool diff);

	std::map<std::string, Stat> const &data() const;
	std::map<std::string, Diff> const &diff() const;

private:
	std::chrono::steady_clock::time_point _load_t;
	std::map<std::string, Stat> _data;
	std::map<std::string, Diff> _diff;
};

}
#endif
