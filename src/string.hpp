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
#ifndef COM_SRC_STRING_HPP_
#define COM_SRC_STRING_HPP_
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace Com {

static inline std::string &toupper(std::string &str)
{
	for (auto &c : str) {
		c = (char)std::toupper(c);
	}
	return str;
}

static inline std::string &tolower(std::string &str)
{
	for (auto &c : str) {
		c = (char)std::tolower(c);
	}
	return str;
}

/// remove all `trim_chars` at front
template <typename... Args>
static inline std::basic_string_view<Args...> trim_front(std::basic_string_view<Args...> const &str, std::basic_string_view<Args...> const &trim_chars)
{
	auto pos = std::find_if_not(str.begin(), str.end(), [&trim_chars](auto c) { return trim_chars.find(c) < trim_chars.size(); });
	return std::basic_string_view<Args...>(pos, str.cend() - pos);
}

/// remove all `trim_chars` at back
template <typename... Args>
static inline std::basic_string_view<Args...> trim_back(std::basic_string_view<Args...> const &str, std::basic_string_view<Args...> const &trim_chars)
{
	auto pos = std::find_if_not(str.rbegin(), str.rend(), [&trim_chars](auto c) { return trim_chars.find(c) < trim_chars.size(); }) - str.rbegin();
	return std::basic_string_view(str.begin(), str.end() - pos - str.begin());
}

/// remove all `trim_chars` at front and back
template <typename... Args>
static inline std::basic_string_view<Args...> trim(std::basic_string_view<Args...> const &str, std::basic_string_view<Args...> const &trim_chars)
{
	return trim_front(trim_back(str, trim_chars), trim_chars);
}

/// remove all white spaces (' ', '\n', '\t', '\v') at front
static inline std::string_view trim_front(std::string_view const &str)
{
	return trim_front(str, std::string_view(" \n\v\t"));
}

/// remove all white spaces (' ', '\n', '\t', '\v') at back
static inline std::string_view trim_back(std::string_view const &str)
{
	return trim_back(str, std::string_view(" \n\v\t"));
}

/// remove all white spaces (' ', '\n', '\t', '\v') at front and back
static inline std::string_view trim(std::string_view const &str)
{
	return trim(str, std::string_view(" \n\v\t"));
}

/// remove all white spaces (' ', '\n', '\t', '\v') at front
static inline std::u32string_view trim_front(std::u32string_view const &str)
{
	return trim_front(str, std::u32string_view(U" \n\v\t"));
}

/// remove all white spaces (' ', '\n', '\t', '\v') at back
static inline std::u32string_view trim_back(std::u32string_view const &str)
{
	return trim_back(str, std::u32string_view(U" \n\v\t"));
}

/// remove all white spaces (' ', '\n', '\t', '\v') at front and back
static inline std::u32string_view trim(std::u32string_view const &str)
{
	return trim(str, std::u32string_view(U" \n\v\t"));
}

/// join two string with delimter
/// if one string is empty then delimter will not be inserted
template <typename Delimter, typename Char>
inline std::basic_string<Char> join(
	Delimter const &delimter,
	std::basic_string_view<Char> lhs,
	std::basic_string_view<Char> rhs
)
{
	if (lhs.empty()) {
		return std::basic_string<Char>(rhs.begin(), rhs.end());
	}
	if (rhs.empty()) {
		return std::basic_string<Char>(lhs.begin(), lhs.end());
	}
	auto ret = std::basic_string<Char>(lhs.begin(), lhs.end());
	ret += delimter;
	ret += rhs;
	return ret;
}

template <typename Delimter, typename Char>
inline std::basic_string<Char> join(
	Delimter const &delimter,
	std::basic_string<Char> const &lhs,
	std::basic_string<Char> const &rhs
)
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

/// split `str` at `each` one of `delims`
template <typename Delims, typename... Args>
inline auto split(std::basic_string_view<Args...> const &str, Delims &&delims, bool each = false)
{
	std::vector<std::basic_string_view<Args...>> strs;
	size_t e = 0;
	while (e < str.size()) {
		auto b = str.find_first_of(delims, e);
		if (b == str.npos) {
			strs.push_back({str.data() + e, str.size() - e});
			break;
		}
		std::basic_string_view<Args...> item(str.data() + e, b - e);
		if (each || !item.empty()) {
			strs.push_back(item);
		}
		e = b + 1;
	}
	return strs;
}

}
#endif
