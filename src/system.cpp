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
#include "system.hpp"
#include "Proc/load.hpp"
#include "string.hpp"
#include <charconv>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace Com {

std::map<unsigned, Passwd> get_passwd()
{
	std::map<unsigned, Passwd> ret;
	auto data = Proc::load_values_style("/etc/passwd", ":");
	for (auto const &items : data) {
		if (items.size() != 7) {
			continue;
		}
		Passwd p;
		p.name = items[0];
		p.password = items[1];
		std::from_chars(&items[2][0], &items[2][0] + items[2].size(), p.uid);
		std::from_chars(&items[3][0], &items[3][0] + items[3].size(), p.gid);
		p.desc = items[4];
		p.home = items[5];
		p.shell = items[6];
		ret.emplace(p.uid, std::move(p));
	}
	return ret;
}

bool get_file_owner(std::filesystem::path const &path, bool follow_link, unsigned &uid, unsigned &gid)
{
	struct stat s;
	if (follow_link) {
		if (stat(path.c_str(), &s)) {
			return false;
		}
	} else {
		if (lstat(path.c_str(), &s)) {
			return false;
		}
	}
	uid = s.st_uid;
	gid = s.st_gid;
	return true;
}

void getuid(unsigned *real, unsigned *effective)
{
	if (real) {
		*real = ::getuid();
	}
	if (effective) {
		*effective = ::geteuid();
	}
}

std::map<unsigned, Group> get_groups()
{
	std::map<unsigned, Group> ret;
	auto data = Proc::load_values_style("/etc/group", ":");
	for (auto const &items : data) {
		if (items.size() < 3) {
			continue;
		}
		Group g;
		g.name = items[0];
		g.password = items[1];
		std::from_chars(&items[2][0], &items[2][0] + items[2].size(), g.gid);
		if (items.size() > 3 && !items[3].empty()) {
			for (auto u : Com::split(items[3], ",")) {
				if (u.empty()) {
					continue;
				}
				g.users.push_back(std::string(u));
			}
		}
		ret.emplace(g.gid, std::move(g));
	}
	return ret;
}

}
