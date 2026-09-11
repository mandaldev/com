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
#ifndef COM_SRC_SYSTEM_HPP
#define COM_SRC_SYSTEM_HPP
#include <filesystem>
#include <map>
#include <string>

namespace Com {

struct Passwd {
	std::string name;
	std::string password;
	unsigned uid;
	unsigned gid;
	std::string desc;
	std::filesystem::path home;
	std::filesystem::path shell;
};

/// <uid, passwd entry>
std::map<int, Passwd> get_passwd();

bool get_file_owner(std::filesystem::path const &path, bool follow_link, unsigned &uid, unsigned &gid);

void getuid(unsigned *real, unsigned *effective);

}
#endif
