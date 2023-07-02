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
#ifndef _COM_SRC_FLAGS_HPP_
#define _COM_SRC_FLAGS_HPP_
#include <type_traits>

namespace Com {

/// a simple flags type.
/// `E` must be an enum type,
/// one enumeration represents single bit, 64 bits at most.
///
/// Flags or called bitmask, usually written as bunch of integer constants
/// that each has only one bit '1'. And to use these bitmasks, you have to
/// use ugly bit operators. With `Flags` you can avoid all that.
///
/// First you use `enum` to define all bitmask: with enum as integer, it's
/// value stands for which bit. For example:
///
/// ```c++
/// enum class Key {
/// 	A = 0, // first must be zero, as the lowest bit
/// 	B, // second bit
/// 	C, // third bit
/// 	// and go on
/// };
/// // keys that are golding
/// using HoldingKeys = Flags<Key>;
/// ```
///
/// NOTE: first enumeration must be zero, as the lowest bit
template <typename E>
struct Flags
{
	uint64_t v = 0; // 64 bits at most

	/// implict convert to integer so you can do all the
	/// bit operations
	constexpr operator uint64_t&()
	{
		return v;
	}

	constexpr Flags()
	{
		static_assert(std::is_unsigned_v<std::underlying_type_t<E>>);
	}

	constexpr Flags(uint64_t flags) : v(flags)
	{}

	constexpr Flags(E e) : v(1ul << (unsigned)e)
	{}

	template <typename... Es> constexpr Flags(E e, Es... es)
	: v(Flags(e) | Flags(es...))
	{}

	constexpr bool test(E e) const
	{
		return v & Flags(e);
	}

	constexpr Flags &set(E e, bool value = true)
	{
		v = value ? (v | Flags(e)) : (v & ~Flags(e));
		return *this;
	}
};

}
#endif
