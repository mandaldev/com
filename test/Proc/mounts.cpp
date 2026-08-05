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
#include "../../src/Proc/Mounts.hpp"

TEST(mounts, "Proc Mounts test")
{
	Com::Proc::Mounts mounts;
	mounts.load();
	auto &data = mounts.get_data();
	auto i = mounts.find_mount_location("/home");
	assert(data.size() > i);

	i = mounts.find_mount_location("/tmp");
	assert(data.size() > i);

	i = mounts.find_mount_location("/boot/efi");
	assert(data.size() > i);
}
