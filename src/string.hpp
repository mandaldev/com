// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2023  Mandal Dev
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
#ifndef _COM_SRC_STRING_HPP_
#define _COM_SRC_STRING_HPP_
#include <string>

namespace Com {

static inline std::string string_uppercase_all(std::string str)
{
	for (auto &c : str) {
		c = std::toupper(c);
	}
	return str;
}

static inline std::string string_lowercase_all(std::string str)
{
	for (auto &c : str) {
		c = std::tolower(c);
	}
	return str;
}

}
#endif
