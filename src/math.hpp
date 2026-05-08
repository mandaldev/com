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
#ifndef COM_SRC_MATH_HPP
#define COM_SRC_MATH_HPP

#include <cmath>
#include <numeric>
namespace Com::Math {

template <typename ItBegin, typename ItEnd>
auto average(ItBegin b, ItEnd e)
{
	using V = decltype(*b - *b);
	auto zero = V(0);
	auto sum = std::accumulate(b, e, zero);
	return b != e ? sum / (e - b) : zero;
}

template <typename ItBegin, typename ItEnd>
auto deviation(ItBegin b, ItEnd e)
{
	using V = decltype(*b - *b);
	auto zero = V(0);
	auto avg = average(b, e);
	auto q_sum = zero;
	for (auto i = b; i != e; i++) {
		q_sum += (*i - avg) * (*i - avg);
	}
	return b != e ? q_sum / (e - b) : zero;
}

template <typename ItBegin, typename ItEnd>
auto std_deviation(ItBegin b, ItEnd e)
{
	return std::sqrt(deviation(b, e));
}

}
#endif
