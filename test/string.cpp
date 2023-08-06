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
#include "../src/unit.hpp"
#include "../src/string.hpp"

TEST(string, "string utils test")
{
	std::string a = "hello", b = "world";
	assert(Com::join(' ', a, b) == "hello world");

	std::vector<std::string> strs = {"a", "b", "c"};
	assert(Com::join(' ', strs) == "a b c");

	{
		std::vector<std::string_view> strs = {"a", "b", "c"};
		assert(Com::split(std::string_view(" a b  c"), " ") == strs);
	}
	{
		std::vector<std::string_view> strs = {"", "a", "b", "", "c"};
		assert(Com::split(std::string_view(" a b  c"), " ", true) == strs);
	}

	strs = {"", "b", "c"};
	assert(Com::join(' ', strs) == "b c");

	assert(Com::trim(std::string_view(" \t\v\nhello world\n")) == "hello world");

	assert(Com::toupper(a) == "HELLO");
	assert(Com::tolower(a) == "hello");
}
