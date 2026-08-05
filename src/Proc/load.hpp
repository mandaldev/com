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
#ifndef COM_SRC_PROC_COMMASTYLELOADER_HPP
#define COM_SRC_PROC_COMMASTYLELOADER_HPP
#include <filesystem>
#include <string>
#include <vector>

namespace Com::Proc {

/// "header <SEP> value"
std::vector<std::pair<std::string, std::string>>
load_header_value_style(std::filesystem::path const &path, std::string const &sep);

/// "header <SEP> value1 <SEP> value2..."
std::vector<std::vector<std::string>>
load_header_values_style(std::filesystem::path const &path, std::string const &sep);

/// "value1 <SEP> value2..."
std::vector<std::vector<std::string>>
load_values_style(std::filesystem::path const &path, std::string const &sep);

}
#endif
