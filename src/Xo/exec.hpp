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
#ifndef CGEN_SRC_XO_EXEC_HPP
#define CGEN_SRC_XO_EXEC_HPP
#include <string>
#include <filesystem>

namespace Com {

/// Exec cmd with `system` call.
/// `exec` use shell '>' to redirect stdout and stderr to temp files,
/// so `cmd` should not use '>' or '|', etc.
int exec(std::string const &cmd, std::string const &tmpname, std::string &stdout_content, std::string &stderr_content);

/// get target path of `/proc/self/exe`
std::filesystem::path exec_path();

/// like `/usr/bin/cgen`, return `/usr`
std::filesystem::path exec_install_prefix();

} // namespace XO

#endif
