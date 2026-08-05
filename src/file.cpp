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
#include "file.hpp"
#include <exception>
#include <fstream>
#include <iostream>

namespace Com {

bool mkdirs(std::filesystem::path const &dir)
{
	std::error_code ec{};
	std::filesystem::create_directories(dir, ec);
	if (ec) {
		return false;
	}
	return true;
}

std::optional<std::string> load_file(std::filesystem::path const &p)
try {
	long const buf_size = 8 << 10;
	auto buf = std::string(buf_size, 0);

	std::string ret;
	auto ifs = std::ifstream(p);
	while (ifs) {
		ifs.read(buf.data(), buf_size);
		auto n = ifs.gcount();
		if (n > 0) {
			ret.insert(ret.end(), buf.begin(), buf.begin() + n);
		}
	}

	return ret;
} catch (std::exception &e) {
	return {};
}

// load mas file
std::optional<Com::Mas::Value> load_mas_file(std::filesystem::path const &path)
{
	auto buf = load_file(path);
	if (buf) {
		return Com::Mas::decode(buf.value());
	} else {
		return {};
	}
}

bool save_file(std::filesystem::path const &path, std::string_view buf)
{
	return mkdirs(path.parent_path()) && std::ofstream(path).write(buf.data(), (std::streamsize)buf.size());
}

bool save_pretty_mas_file(std::filesystem::path const &path, Mas::Value const &mas)
{
	Mas::EncodeOption opt;
	opt.pretty = true;
	opt.force_quoting = false;
	return save_file(path, Mas::encode(mas, opt));
}

}
