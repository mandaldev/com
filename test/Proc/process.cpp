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
#include "../../src/unit.hpp"
#include "../../src/Proc/Process.hpp"
#include <cassert>

TEST(process, "Proc Process test")
{
	auto stat = Com::Proc::Process::stat("self");
	assert(stat.pid);
	auto statm = Com::Proc::Process::statm("self");
	assert(statm.size);
	auto page = Com::Proc::Process::page_size();
	assert(page);
	return;
}
