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
#include "Mounts.hpp"
#include "../file.hpp"
#include "../string.hpp"
#include <filesystem>
#include <string_view>
#include <system_error>

namespace Com::Proc {

bool Mounts::load()
{
	_data.clear();
	_map.clear();
	auto opt = load_file("/proc/mounts");
	if (!opt) {
		return false;
	}
	auto lines = split(std::string_view(opt.value()), "\n");
	for (auto line : lines) {
		auto fields = split(line, " ");
		if (fields.size() < 6) {
			continue;
		}
		auto &data = _data.emplace_back();
		data.device = fields[0];
		data.location = fields[1];
		data.fs_type = fields[2];
		for (auto opt : split(fields[3], ",")) {
			data.opts.push_back(std::string(opt));
		}
		data.freq = fields[4];
		data.passno = fields[5];
		_map[data.location] = _data.size() - 1;
	}
	return true;
}

std::vector<Mounts::Mount> const &Mounts::get_data() const
{
	return _data;
}

size_t Mounts::find_mount_location(std::filesystem::path const &path) const
{
	std::error_code ec;
	auto p = std::filesystem::canonical(path, ec);
	if (ec) {
		return _data.size();
	}
	do {
		auto it = _map.find(p);
		if (it != _map.end()) {
			return it->second;
		}
		if (p == "/") {
			break;
		}
		p = p.parent_path();
	} while (true);

	return _data.size();
}

}
