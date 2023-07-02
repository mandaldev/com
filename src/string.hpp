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
#ifndef _COM_SRC_STRING_HPP_
#define _COM_SRC_STRING_HPP_
#include <string>
#include <string_view>
#include <cctype>
#include <vector>
#include <algorithm>
#include "Range.hpp"

namespace Com {

static inline std::string &toupper(std::string &str)
{
	for (auto &c : str) {
		c = std::toupper(c);
	}
	return str;
}

static inline std::string &tolower(std::string &str)
{
	for (auto &c : str) {
		c = std::tolower(c);
	}
	return str;
}

/// remove all white spaces at front
static inline std::string_view trim_front(std::string_view const &str)
{
	auto pos = std::find_if_not(str.begin(), str.end(), [](auto c) { return std::isspace((unsigned char)c); });
	return std::string_view(pos, str.cend() - pos);
}

/// remove all white spaces at back
static inline std::string_view trim_back(std::string_view const &str)
{
	auto pos = std::find_if_not(str.rbegin(), str.rend(), [](auto c) { return std::isspace((unsigned char)c); }) - str.rbegin();
	return std::string_view(str.begin(), str.end() - pos - str.begin());
}

/// remove all white spaces at front and back
static inline std::string_view trim(std::string_view const &str)
{
	return trim_front(trim_back(str));
}

/// join two string with delimter
/// if one string is empty then delimter will not be inserted
template <typename Delimter, typename Item>
inline Item join(Delimter const &delimter, Item const &lhs, Item const &rhs)
{
	if (lhs.empty()) {
		return rhs;
	}
	if (rhs.empty()) {
		return lhs;
	}
	return lhs + delimter + rhs;
}

/// concat strings with `delimter`
/// if one string is empty then delimter will not be inserted
template <typename Delimter, typename Range>
inline auto join(Delimter const &delimter, Range const &range)
{
	auto ret = decltype(join(delimter, *range.begin(), *range.begin()))();
	for (auto &str : range) {
		ret = join(delimter, ret, str);
	}
	return ret;
}

/// split string by `delims`
template <typename Delims, typename... Args>
inline auto split(std::basic_string<Args...> const &str, Delims &&delims)
{
	std::vector<std::basic_string<Args...>> strs;
	size_t e = 0;
	while (e != str.npos) {
		auto b = str.find_first_not_of(delims, e);
		if (b == str.npos) {
			break;
		}
		e = str.find_first_of(delims, b);
		strs.push_back(std::basic_string<Args...>(str.data() + b, str.data() + (e == str.npos ? str.size() : e)));
	}
	return strs;
}

}
#endif
