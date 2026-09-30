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
#include "system.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <mutex>

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

std::filesystem::path root_dir()
{
	return "/";
}

std::filesystem::path home_dir_no_cache()
{
	unsigned uid = 0;
	Com::getuid(nullptr, &uid);
	auto passwds = Com::get_passwd();
	if (auto it = passwds.find(uid); it != passwds.end()) {
		return it->second.home;
	}
	return {};
}

std::filesystem::path home_dir()
{
	static std::once_flag of;
	static std::filesystem::path ret;
	std::call_once(of, []() { ret = home_dir_no_cache(); });
	return ret;
}

bool is_filename_valid(std::string const &name)
{
	auto disable = name.empty() || name == "." || name == ".." || name.find("\\") < name.size();
	return !disable;
}

Com::Flags<FilePerm> permissions_for_me(std::filesystem::path const &path)
{
	Com::Flags<FilePerm> ret;
	std::error_code ec;
	auto stat = std::filesystem::status(path, ec);
	auto perm = stat.permissions();
	unsigned fuid = 0, fgid = 0, uid = 0;
	if (!Com::get_file_owner(path, true, fuid, fgid)) {
		return ret;
	}
	Com::getuid(nullptr, &uid);

	if (uid == fuid) {
		ret.set(FilePerm::R, std::filesystem::perms::none != (perm & std::filesystem::perms::owner_read));
		ret.set(FilePerm::W, std::filesystem::perms::none != (perm & std::filesystem::perms::owner_write));
		ret.set(FilePerm::X, std::filesystem::perms::none != (perm & std::filesystem::perms::owner_exec));
		return ret;
	}

	auto passwds = Com::get_passwd();
	auto it = passwds.find(uid);
	if (it == passwds.end()) {
		return false;
	}

	if (it->second.gid == fgid) {
		ret.set(FilePerm::R, std::filesystem::perms::none != (perm & std::filesystem::perms::group_read));
		ret.set(FilePerm::W, std::filesystem::perms::none != (perm & std::filesystem::perms::group_write));
		ret.set(FilePerm::X, std::filesystem::perms::none != (perm & std::filesystem::perms::group_exec));
		return ret;
	}

	ret.set(FilePerm::R, std::filesystem::perms::none != (perm & std::filesystem::perms::others_read));
	ret.set(FilePerm::W, std::filesystem::perms::none != (perm & std::filesystem::perms::others_write));
	ret.set(FilePerm::X, std::filesystem::perms::none != (perm & std::filesystem::perms::others_exec));
	return ret;
}

bool is_lexically_kid_of(
	std::filesystem::path const &dir,
	std::filesystem::path const &top,
	std::filesystem::path const &kid
)
{
	auto p = kid;
	while (p != "/" && p.has_parent_path()) {
		p = p.parent_path();
		if (p == top) {
			return false;
		}
		if (p == dir) {
			return true;
		}
	}
	return false;
}

std::vector<std::filesystem::path> remove_kids(
	std::vector<std::filesystem::path> const &paths,
	std::filesystem::path const &top
)
{
	size_t tmp = 0;
	std::vector<std::filesystem::path> dst;
	for (auto const &p : paths) {
		if (tmp < dst.size() && Com::is_lexically_kid_of(dst[tmp], top, p)) {
			continue;
		}
		tmp = dst.size();
		dst.push_back(p);
	}
	return dst;
}

std::filesystem::path canonical_parent(std::filesystem::path path, std::error_code ec)
{
	ec = {};
	if (path.is_relative()) {
		path = std::filesystem::absolute(path, ec);
		if (ec) {
			return path;
		}
		path = path.lexically_normal();
		if (!path.has_filename()) {
			path = path.parent_path();
		}
	}
	path = std::filesystem::canonical(path.parent_path(), ec) / path.filename();
	return path;
}

}
