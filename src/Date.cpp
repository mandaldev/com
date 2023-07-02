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
#include "Date.hpp"

namespace Com::Date {

namespace Now {

unsigned int year()
{
	auto time = std::time(nullptr);
	auto tm = *std::localtime(&time);
	return tm.tm_year + 1900;
}

unsigned int month()
{
	auto time = std::time(nullptr);
	auto tm = *std::localtime(&time);
	return tm.tm_mon + 1;
}

unsigned int month_day()
{
	auto time = std::time(nullptr);
	auto tm = *std::localtime(&time);
	return tm.tm_mday;
}

}

}
