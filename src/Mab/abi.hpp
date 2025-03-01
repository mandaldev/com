// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2025  Mandal Dev
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
#ifndef COM_SRC_MAB_ABI_HPP
#define COM_SRC_MAB_ABI_HPP
#include <cstdint>
#include <type_traits>

namespace Com::Mab {

/// mark end point
constexpr uint8_t abi_end = 0;

/// basic type abi, return 0 if T is not supported basic type
template <typename T>
constexpr uint8_t basic_type_abi()
{
	#define IS_ABI(t) constexpr (std::is_same_v<Type, t>) { return __LINE__ - abi_start; }
	using Type = std::remove_cv_t<std::remove_reference_t<T>>;
	const auto abi_start = __LINE__;
	if IS_ABI(bool)
	else if IS_ABI(char)
	else if IS_ABI(int8_t)
	else if IS_ABI(uint8_t)
	else if IS_ABI(int16_t)
	else if IS_ABI(uint16_t)
	else if IS_ABI(int32_t)
	else if IS_ABI(uint32_t)
	else if IS_ABI(int64_t)
	else if IS_ABI(uint64_t)
	else if IS_ABI(float)
	else if IS_ABI(double)
	#undef IS_ABI

	// invalid
	return 0;
}

static_assert(basic_type_abi<double>() == 12, "");

/// enum type abi, return 0 if T is not supported enum type
template <typename T>
constexpr uint8_t enum_type_abi()
{
	using Type = std::remove_cv_t<std::remove_reference_t<T>>;
	#define IS_ENUM_ABI(t) constexpr (std::is_same_v<std::underlying_type_t<Type>, t>) { \
		return __LINE__ - abi_start + basic_type_abi<double>(); \
	}
	if constexpr (std::is_enum_v<Type>) {
		const auto abi_start = __LINE__;
		if IS_ENUM_ABI(int8_t)
		else if IS_ENUM_ABI(uint8_t)
		else if IS_ENUM_ABI(int16_t)
		else if IS_ENUM_ABI(uint16_t)
		else if IS_ENUM_ABI(int32_t)
		else if IS_ENUM_ABI(uint32_t)
		else if IS_ENUM_ABI(int64_t)
		else if IS_ENUM_ABI(uint64_t)
	}
	#undef IS_ENUM_ABI

	// invalid
	return 0;
}

namespace detial_ {

enum E : uint64_t {};

// test
static_assert(enum_type_abi<E>() == 20, "");

}

/// map: key and value can be different type
constexpr uint8_t map_abi = enum_type_abi<detial_::E>() + 1;

/// array: list of different type values
constexpr uint8_t array_abi = map_abi + 1;

/// empty array: same type value array
constexpr uint8_t array_0_abi = array_abi + 1;
/// array: same type value array, followed by a 1 bytes size unsigned integer as length
constexpr uint8_t array_1_abi = array_0_abi + 1;
/// array: same type value array, followed by a 2 bytes size unsigned integer as length
constexpr uint8_t array_2_abi = array_1_abi + 1;
/// array: same type value array, followed by a 3 bytes size unsigned integer as length
constexpr uint8_t array_3_abi = array_2_abi + 1;
/// array: same type value array, followed by a 4 bytes size unsigned integer as length
constexpr uint8_t array_4_abi = array_3_abi + 1;
/// array: same type value array, followed by a 5 bytes size unsigned integer as length
constexpr uint8_t array_5_abi = array_4_abi + 1;
/// array: same type value array, followed by a 6 bytes size unsigned integer as length
constexpr uint8_t array_6_abi = array_5_abi + 1;
/// array: same type value array, followed by a 7 bytes size unsigned integer as length
constexpr uint8_t array_7_abi = array_6_abi + 1;
/// array: same type value array, followed by a 8 bytes size unsigned integer as length
constexpr uint8_t array_8_abi = array_7_abi + 1;

constexpr uint8_t min_abi = 1;
constexpr uint8_t max_abi = array_8_abi;

/// basic and enum types binary size, return 0 if `abi` is invalid
static inline std::size_t scalar_type_size(uint8_t abi)
{
	enum ei8 : int8_t  {};
	enum eu8 : uint8_t  {};
	enum ei16 : int16_t  {};
	enum eu16 : uint16_t  {};
	enum ei32 : int32_t  {};
	enum eu32 : uint32_t  {};
	enum ei64 : int64_t  {};
	enum eu64 : uint64_t  {};
	switch (abi) {
	case basic_type_abi<bool>():
	case basic_type_abi<char>():
	case basic_type_abi<int8_t>():
	case basic_type_abi<uint8_t>():
	case enum_type_abi<ei8>():
	case enum_type_abi<eu8>():
		return 1;
	case basic_type_abi<int16_t>():
	case basic_type_abi<uint16_t>():
	case enum_type_abi<ei16>():
	case enum_type_abi<eu16>():
		return 2;
	case basic_type_abi<int32_t>():
	case basic_type_abi<uint32_t>():
	case enum_type_abi<ei32>():
	case enum_type_abi<eu32>():
	case basic_type_abi<float>():
		static_assert(sizeof(float) == sizeof(int32_t), "float is not 32-bit");
		return 4;
	case basic_type_abi<int64_t>():
	case basic_type_abi<uint64_t>():
	case enum_type_abi<ei64>():
	case enum_type_abi<eu64>():
	case basic_type_abi<double>():
		static_assert(sizeof(double) == sizeof(int64_t), "double is not 64-bit");
		return 8;
	default:
		return 0;
	}
}

}


#endif
