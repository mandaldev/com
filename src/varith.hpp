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
#ifndef COM_SRC_VARITH_HPP
#define COM_SRC_VARITH_HPP
#include "meta.hpp"
#include <algorithm>
#include <cmath>

template <typename C, typename S>
std::enable_if_t<Com::IsSequenceOf<C, S>::value, C>
operator*
(C const &vec, S scalar)
{
	auto ret = vec;
	for (auto &v : ret) {
		v *= scalar;
	}
	return ret;
}

template <typename C, typename S>
std::enable_if_t<Com::IsSequenceOf<C, S>::value, C>
operator/
(C const &vec, S scalar)
{
	return vec * (S(1) / scalar);
}

template <typename C, typename S>
std::enable_if_t<Com::IsSequenceOf<C, S>::value, C>
operator+
(C const &vec, S scalar)
{
	auto ret = vec;
	for (auto &v : ret) {
		v += scalar;
	}
	return ret;
}

template <typename C, typename S>
std::enable_if_t<Com::IsSequenceOf<C, S>::value, C>
operator-
(C const &vec, S scalar)
{
	return vec + -scalar;
}

template <typename C>
std::enable_if_t<Com::IsSequence<C>::value, C>
operator+
(C const &vec1, C const &vec2)
{
	auto ret = vec1;
	auto it = ret.begin();
	auto it1 = vec1.begin();
	auto it2 = vec2.begin();
	while (it1 != vec1.end() && it2 != vec2.end()) {
		*it = *it1 + *it2;
		++it;
		++it1;
		++it2;
	}
	return ret;
}

template <typename C>
std::enable_if_t<Com::IsSequence<C>::value, C>
operator-
(C const &vec1, C const &vec2)
{
	return vec1 + vec2 * -1;
}

namespace Com {

template <typename C>
std::enable_if_t<IsSequence<C>::value, C>
round(C const &vec)
{
	auto ret = vec;
	for (auto &v : ret) {
		v = std::round(v);
	}
	return ret;
}

template <typename C, typename S>
std::enable_if_t<IsSequenceOf<C, S>::value, C>
max(C const &vec, S s)
{
	auto ret = vec;
	for (auto &v : ret) {
		v = std::max(v, s);
	}
	return ret;
}

template <typename C, typename S>
std::enable_if_t<IsSequenceOf<C, S>::value, C>
min(C const &vec, S s)
{
	auto ret = vec;
	for (auto &v : ret) {
		v = std::min(v, s);
	}
	return ret;
}

template <typename C, typename S>
std::enable_if_t<IsSequenceOf<C, S>::value, C>
clamp(C const &vec, S min, S max)
{
	auto ret = vec;
	for (auto &v : ret) {
		v = std::clamp(v, min, max);
	}
	return ret;
}

}
#endif
