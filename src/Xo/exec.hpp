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
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace Com {

/// Exec cmd with `system` call.
/// `exec` use shell '>' to redirect stdout and stderr to temp files,
/// so `cmd` should not use '>' or '|', etc.
int exec(std::string const &cmd, std::string const &tmpname, std::string &stdout_content, std::string &stderr_content);

/// get target path of `/proc/self/exe`
std::filesystem::path exec_path();

/// like `/usr/bin/cgen`, return `/usr`
std::filesystem::path exec_install_prefix();

/// environ 的 map 版本
std::map<std::string, std::string> build_env_map();

/// 创建新的进程并执行程序。
/// - `program` ：可执行文件名或路径
/// - `args` ：参数，注意第一个参数是命令的名字或路径
/// - `env` ：环境变量，为空则使用当前全局变量 `environ`
/// - `child_pid` ：创建成功返回子进程 id
std::error_code fork_and_exec(
	std::filesystem::path const &program,
	std::vector<std::string> args,
	std::map<std::string, std::string> const &env = {},
	int *child_pid = nullptr
);

} // namespace XO

#endif
