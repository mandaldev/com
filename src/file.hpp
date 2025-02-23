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
#include <filesystem>
#include <optional>

namespace Com {

bool mkdirs(std::filesystem::path const &dir);

std::optional<std::string> load_file(std::filesystem::path const &p);

std::optional<Com::Mas::Value> load_mas_file(std::filesystem::path const &path);

bool save_file(std::filesystem::path const &path, std::string_view buf);

bool save_pretty_mas_file(std::filesystem::path const &path, Mas::Value const &mas);

}
#endif
