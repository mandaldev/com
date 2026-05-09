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
#include <fstream>
#include <iostream>

namespace Com {

bool mkdirs(std::filesystem::path const &dir)
{
	std::error_code ec{};
	std::filesystem::create_directories(dir, ec);
	if (ec) {
		std::cerr << "Error: failed to create directory " << dir << ", error: " << ec << "\n";
		return false;
	}
	return true;
}

std::optional<std::string> load_file(std::filesystem::path const &p)
{
	std::error_code ec {};
	auto size = std::filesystem::file_size(p, ec);
	if (ec) {
		return {};
	}
	if (size == 0) {
		return std::string();
	}
	auto buf = std::string(size, 0);
	auto ifs = std::ifstream(p);
	return ifs.read(buf.data(), (std::streamsize)buf.size()) ? buf : std::string();
}

// load mas file
std::optional<Com::Mas::Value> load_mas_file(std::filesystem::path const &path)
{
	auto buf = load_file(path);
	if (buf) {
		return Com::Mas::decode(buf.value());
	} else {
		std::cerr << "Error: read " << path << " failed\n";
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
