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
#include "exec.hpp"
#include "../file.hpp"
#include "../string.hpp"
#include <cassert>
#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <unistd.h>
#include <vector>
using namespace std::literals;

static inline std::string get_tmpnam(std::string const &tmpname)
{
	int n = 0;
	auto name = tmpname + "_a";
	while (n++ < 20) {
		std::error_code ec{};
		if (!std::filesystem::exists(name, ec) && !ec) { // no one is using it
			return name;
		}
		name.back() += 1;
	}
	return {};
}

namespace Com {

int exec(std::string const &cmd, std::string const &tmpname, std::string &stdout_content, std::string &stderr_content)
{
	std::string cmdline = cmd;
	std::string out_name = get_tmpnam(tmpname + "_stdout");
	std::string err_name = get_tmpnam(tmpname + "_stderr");
	if (out_name.empty() || err_name.empty()) {
		stderr_content = "no more temp files";
		return 1;
	}
	cmdline += " &> " + err_name + " > " + out_name;
	cmdline = "bash -c '" + cmdline + "'";
	auto ret = std::system(cmdline.c_str());
	if (ret) {
		// failed
		try {
			stderr_content = load_file(err_name).value();
		} catch (std::exception const &e) {
			stderr_content = "Failed to read output: ";
			stderr_content += e.what();
			ret = 1;
		}
	} else {
		try {
			stdout_content = load_file(out_name).value();
		} catch (std::exception const &e) {
			stderr_content = "Failed to read output: ";
			stderr_content += e.what();
			ret = 1;
		}
	}
	std::system(("rm -f " + err_name + " " + out_name).c_str());
	return ret;
}

std::filesystem::path exec_path()
{
	std::error_code ec{};
	return std::filesystem::read_symlink("/proc/self/exe", ec);
}

std::filesystem::path exec_install_prefix()
{
	auto p = exec_path();
	if (p.empty()) {
		return p;
	}
	auto name = p.filename();
	auto prefix = p.parent_path().parent_path();
	if (prefix / "bin" / name == p) {
		return prefix;
	}
	return {};
}

std::map<std::string, std::string> build_env_map()
{
	std::map<std::string, std::string> ret;
	for (auto p = environ; *p; p++) {
		auto kv = Com::split(std::string_view(*p), "=");
		if (kv.size() == 2) {
			ret[std::string(kv[0])] = kv[1];
		}
	}
	return ret;
}

std::error_code fork_and_exec(
	std::filesystem::path const &program,
	std::vector<std::string> args,
	std::map<std::string, std::string> const &env,
	int *child_pid
)
{
	if (!program.has_filename()) {
		return std::make_error_code(std::errc::invalid_argument);
	}
	auto pid = fork();
	switch (pid) {
	case -1: { // error
		return std::make_error_code((std::errc)errno);
	}
	case 0: { // child
		break;
	}
	default: { // parent
		if (child_pid) {
			*child_pid = pid;
		}
		return {};
	}
	}

	auto p = program.string();

	std::vector<char*> a;
	a.reserve(args.size() + 1);
	for (auto &v : args) {
		v.push_back(0);
		a.push_back(v.data());
	}
	a.push_back(nullptr);

	if (env.empty()) {
		execvp(p.c_str(), a.data());
	} else {
		std::vector<char*> ps;
		ps.reserve(env.size() + 1);
		std::vector<std::string> ss;
		ss.reserve(env.size());
		for (auto &[k, v] : env) {
			ps.push_back(ss.emplace_back(k + "=" + v + '\0').data());
		}
		ps.push_back(nullptr);
		execvpe(p.c_str(), a.data(), ps.data());
	}

	return std::make_error_code((std::errc)errno);
}

} // namespace XO
