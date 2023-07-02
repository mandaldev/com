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
#ifndef _COM_SRC_RANGE_HPP_
#define _COM_SRC_RANGE_HPP_

namespace Com {

/// Wrap `c.begin(), c.end()` as a container
/// This `Range` is very basic, since iterators wrapped could be
/// any type.
/// Mostly use in range-for loops.
template <typename Begin, typename End>
class Range
{
public:
	Range(Begin b, End e)
	: _b(b)
	, _e(e)
	{
	}

	auto begin() const
	{
		return _b;
	}

	auto end() const
	{
		return _e;
	}

	auto size() const
	{
		return _e - _b;
	}

	bool empty() const
	{
		return _b == _e;
	}

	void clear()
	{
		_b = _e;
	}

private:
	Begin _b;
	End _e;
};

template <typename Begin, typename End>
inline auto make_range(Begin b, End e)
{
	return Range<Begin, End>(b, e);
}

}
#endif
