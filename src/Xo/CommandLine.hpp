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
#ifndef MAS_SRC_COMMANDLINE_HPP
#define MAS_SRC_COMMANDLINE_HPP
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace Com {

/// A command line like this:
///
///   exec-name -switch ... --switch=value ... arg1 arg2 ...
///
/// No matter of orderness of switchs and arguments.
class CommandLine
{
	int _argc = 0;
	char const *const *_argv = nullptr;

	bool _valid_syntax = false;
	std::vector<std::string> _args;
	std::map<std::string, std::string> _switchs;

	void parse();

public:

	CommandLine(int argc, char const *const *argv);

	bool is_valid() const;

	bool has_switch(std::string const &name) const;

	std::optional<std::string> get_switch(std::string const &name) const;

	std::map<std::string, std::string> get_switchs() const;

	std::vector<std::string> get_arguments() const;

}; // class CommandLine

} // namespace XO

#endif
