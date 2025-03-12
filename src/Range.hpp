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
#ifndef COM_SRC_RANGE_HPP_
#define COM_SRC_RANGE_HPP_
#include "meta.hpp"
#include <cstdint>
#include <iterator>

namespace Com {

template <typename Begin, typename End>
auto make_range(Begin b, End e);

/// Wrap `c.begin(), c.end()` as a container
/// This `Range` is very basic, since iterators wrapped could be
/// any type.
/// Mostly use in range-for loops.
template <typename Begin, typename End>
class Range
{
public:
	Range(Begin b, End e) : _b(b) , _e(e) {}

	template <typename T>
	Range(T *p, size_t count)
	: _b(p)
	, _e(p + count)
	{
	}

	auto sub_range(size_t offset, size_t count) const
	{
		assert(offset + count >= size());
		auto b = _b;
		std::advance(b, offset);
		auto e = b;
		std::advance(e, count);
		return make_range(b, e);
	}

	auto begin() const { return _b; }
	auto end() const { return _e; }

	auto rbegin() const { return std::prev(_e); }
	auto rend() const { return std::prev(_b); }

	auto data() const { return &(*_b); }

	auto &operator[](std::size_t index)
	{
		auto b = _b;
		std::advance(b, index);
		return *b;
	}

	size_t size() const { return (size_t)std::abs(std::distance(_b, _e)); }

	bool empty() const { return _b == _e; }

	void clear() { _b = _e; }

private:
	Begin _b;
	End _e;
};

template <typename Begin, typename End>
auto make_range(Begin b, End e)
{
	return Range<Begin, End>(b, e);
}

}
#endif
