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
#ifndef _COM_SRC_MEMORY_HPP_
#define _COM_SRC_MEMORY_HPP_
#include <memory>

namespace Com {

template <typename T>
static inline auto make(std::shared_ptr<T> &sp)
{
	return sp = std::make_shared<T>();
}

template <typename T, typename... Args>
static inline auto make(std::shared_ptr<T> &sp, Args&&... args)
{
	return sp = std::make_shared<T>(std::forward(args...));
}

}
#endif
