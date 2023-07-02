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
#ifndef _COM_SRC_RAII_HPP_
#define _COM_SRC_RAII_HPP_
#include <functional>

namespace Com {

// delete copy traits
class Unique
{
protected:
	// no direct use of Unique
	~Unique() = default;
public:
	Unique() = default;
	Unique(Unique const &) = delete;
	Unique &operator=(Unique const &) = delete;
};

class Scope
: public Unique
{
	std::function<void()> _on_scope_out;
public:
	Scope(std::function<void()> &&on_scope_out)
	: _on_scope_out(on_scope_out)
	{}

	~Scope()
	{
		if (_on_scope_out) {
			_on_scope_out();
		}
	}
};

}
#endif
