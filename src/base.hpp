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
#ifndef COM_SRC_BASE_HPP_
#define COM_SRC_BASE_HPP_
#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>

namespace Com {

template <typename T>
static inline constexpr bool is_in_range(T v, T min, T max)
{
	return v >= min && v <= max;
}

template <typename T>
static inline constexpr bool is_in_range_open(T v, T min, T max)
{
	return v > min && v < max;
}

template <typename T>
static inline constexpr bool is_in_range_openl(T v, T min, T max)
{
	return v > min && v <= max;
}

template <typename T>
static inline constexpr bool is_in_range_openr(T v, T min, T max)
{
	return v >= min && v < max;
}

static inline constexpr uint32_t make_int(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4)
{
	return (i4 << 24) | (i3 << 16) | (i2 << 8) | i1;
}

/// static cast values
template <typename ItDst, typename ItSrc>
static inline void static_cast_range(ItDst dst, ItSrc begin, ItSrc end)
{
	while (begin != end) {
		*dst++ = static_cast<std::remove_reference_t<decltype(*dst)>>(*begin++);
	}
}

/// cast `v` to signed number with range clamped
template <typename T>
static inline constexpr std::make_signed_t<T> sign(T v)
{
	if (std::is_signed_v<T>) {
		return v;
	}
	return std::min(v, (T)std::numeric_limits<std::make_signed_t<T>>::max());
}

/// cast `v` to unsigned number with range clamped
template <typename T>
static inline constexpr std::make_unsigned_t<T> unsign(T v)
{
	if (std::is_unsigned_v<T>) {
		return v;
	}
	if (v < 0) {
		return 0;
	}
	return v;
}

/// return empty optional if `ec`
template <typename T, typename E>
static inline std::optional<T> optional_when_error(T &&value, E const &ec)
{
	if (ec) {
		return {};
	}
	return value;
}

/// return empty optional if `ec` and reset `ec`
template <typename T, typename E>
static inline std::optional<T> error_to_optional(T &&value, E &ec)
{
	if (ec) {
		ec = {};
		return {};
	}
	return value;
}

template <typename Fn>
static inline void repeat(size_t n, Fn &&fn)
{
	for (size_t i = 0; i < n; i++) {
		fn();
	}
}

}
#endif
