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
static inline void make(std::shared_ptr<T> &sp)
{
	sp = std::make_shared<T>();
}

template <typename T, typename... Args>
static inline void make(std::shared_ptr<T> &sp, Args&&... args)
{
	sp = std::make_shared<T>(std::forward<Args>(args)...);
}

/// as `std::enable_shared_from_this`, and add `bind_weak` api
/// to provide safer callbacks.
/// 一般来说，捕获共享指针是个糟糕的做法——可能引起循环引用，导致内存泄漏。
/// 所以应该捕获弱指针，函数体内再获取共享指针。
/// 这个类是对这种做法的一个包装，避免编写重复的设计代码。
template <typename T>
class EnableBindWeak
: public std::enable_shared_from_this<T>
{
public:
	/// wrap a lambda with another lambda
	template <typename Fn>
	auto bind_weak(Fn &&fn)
	{
		auto weak = this->weak_from_this();
		return [weak, fn](auto&&... args)
		{
			auto shared = weak.lock();
			if (!shared) {
				return;
			}
			return fn(std::forward<decltype(args)>(args)...);
		};
	}
};


}
#endif
