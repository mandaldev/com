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
#ifndef COM_SRC_FILE_HPP_
#define COM_SRC_FILE_HPP_
#include "Mas/Value.hpp"
#include "Flags.hpp"
#include <filesystem>
#include <optional>

namespace Com {

bool mkdirs(std::filesystem::path const &dir);

std::optional<std::string> load_file(std::filesystem::path const &p);

std::optional<Com::Mas::Value> load_mas_file(std::filesystem::path const &path);

bool save_file(std::filesystem::path const &path, std::string_view buf);

bool save_pretty_mas_file(std::filesystem::path const &path, Mas::Value const &mas);

/// 根目录 （“/”）
std::filesystem::path root_dir();

/// 当前用户主目录（不使用全局缓存的，如果有更改运行时用户的可能，调次函数）
std::filesystem::path home_dir_no_cache();

/// 当前用户主目录
std::filesystem::path home_dir();

/// 文件名是否合法（仅检查字符）
bool is_filename_valid(std::string const &name);

enum class FilePerm : unsigned {
	R = 0,
	W,
	X,
};

/// 当前用户所拥有的 `path` 所指文件的权限
Flags<FilePerm> permissions_for_me(std::filesystem::path const &path);

/// 检查 kid 是否位于目录 dir 下面（直接或间接）。
/// 如果 top 不是空，则认为 kid 和 dir 都在 top 目录下面，则可以节省一些计算。
bool is_lexically_kid_of(
	std::filesystem::path const &dir,
	std::filesystem::path const &top,
	std::filesystem::path const &kid
);

}
#endif
