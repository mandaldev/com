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
#include "utf8.hpp"
#include "../base.hpp"
#include <array>
#include <cassert>

namespace Com::Codecs {

/// UTF8-octets = *( UTF8-char )
/// UTF8-char   = UTF8-1 / UTF8-2 / UTF8-3 / UTF8-4
///
/// UTF8-1      = %x00-7F
///
/// UTF8-2      = %xC2-DF UTF8-tail
///
/// UTF8-3      = %xE0 %xA0-BF UTF8-tail /
///               %xE1-EC 2( UTF8-tail ) /
///               %xED %x80-9F UTF8-tail /
///               %xEE-EF 2( UTF8-tail )
///
/// UTF8-4      = %xF0 %x90-BF 2( UTF8-tail ) /
///               %xF1-F3 3( UTF8-tail ) /
///               %xF4 %x80-8F 2( UTF8-tail )
///
/// UTF8-tail   = %x80-BF

#define IN_RANGE(c, min, max) Com::is_in_range((int)c, min, max)
#define IS_TAIL(c) (OCT_TAIL[(int)c])

const static uint8_t OCT_TAIL[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// 80-BF
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

const static uint8_t OCT_HEAD[256] = {
	// 00-7F: 0YYY YYYY, 7F
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	// 80-C1: invalid
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0,
	// C2-DF: 110Y YYYx, 1F
	      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	// E0
	31,
	// E1-EC
	   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	// ED
	   33,
	// EE-EF
	   32, 32,
	// F0
	41,
	// F1-F3
	   42, 42, 42,
	// F4
	               43,
};

static inline size_t decode_one(const uint8_t *u, size_t len, char32_t *cp)
{
	if (!len) {
		return 0;
	}
	switch (OCT_HEAD[(int)u[0]]) {
	case 1:
		goto OCT1;
	case 2:
		if (len > 1 && IS_TAIL(u[1]))
			goto OCT2;
		break;
	case 31:
		if (len > 2 && IN_RANGE(u[1], 0xA0, 0xBF) && IS_TAIL(u[2]))
			goto OCT3;
		break;
	case 32:
		if (len > 2 && IS_TAIL(u[1]) && IS_TAIL(u[2]))
			goto OCT3;
		break;
	case 33:
		if (len > 2 && IN_RANGE(u[1], 0x80, 0x9F) && IS_TAIL(u[2]))
			goto OCT3;
		break;
	case 41:
		if (len > 3 && IN_RANGE(u[1], 0x90, 0xBF) && IS_TAIL(u[2]) && IS_TAIL(u[3]))
			goto OCT4;
		break;
	case 42:
		if (len > 3 && IS_TAIL(u[1]) && IS_TAIL(u[2]) && IS_TAIL(u[3]))
			goto OCT4;
		break;
	case 43:
		if (len > 3 && IN_RANGE(u[1], 0x80, 0x8F) && IS_TAIL(u[2]) && IS_TAIL(u[3]))
			goto OCT4;
		break;
	default:
		break;
	}
	return 0;
OCT1:
	*cp = u[0];
	return 1;
OCT2:
	*cp = (u[0] & 0x1F) << 6;
	*cp += u[1] & 0x3F;
	return 2;
OCT3:
	*cp = (u[0] & 0x0F) << 12;
	*cp += (u[1] & 0x3F) << 6;
	*cp += u[2] & 0x3F;
	return 3;
OCT4:
	*cp = (u[0] & 0x07) << 18;
	*cp += (u[1] & 0x3F) << 12;
	*cp += (u[2] & 0x3F) << 6;
	*cp += u[3] & 0x3F;
	return 4;
}

static inline size_t encode_one(char32_t cp, std::array<uint8_t, 5> &u)
{
	if (!(cp >> 7u)) {
		u[1] = 0;
		u[0] = cp;
		return 1;
	} else if (!(cp >> 11u)) {
		u[2] = 0;
		u[1] = 0x80u | (0x3Fu & cp);
		u[0] = 0xC0u | (cp >> 6u);
		return 2;
	} else if (!(cp >> 16)) {
		u[3] = 0;
		u[2] = 0x80u | (0x3Fu & cp);
		u[1] = 0x80u | (0x3Fu & (cp >> 6));
		u[0] = 0xE0u | (cp >> 12);
		return 3;
	} else if (!(cp >> 21u)) {
		u[4] = 0;
		u[3] = 0x80u | (0x3Fu & cp);
		u[2] = 0x80u | (0x3Fu & (cp >> 6));
		u[1] = 0x80u | (0x3Fu & (cp >> 12));
		u[0] = 0xF0u | (cp >> 18);
		return 4;
	}
	return 0;
}

std::optional<std::u32string> utf8_decode(std::string_view str)
{
	std::u32string out;
	out.reserve(str.length());
	while (str.length()) {
		char32_t cp;
		auto n = decode_one((uint8_t const*)str.data(), str.length(), &cp);
		str.remove_prefix(n);
		if (n == 0) {
			assert(!"codec::utf8 decode: bad byte sequency!");
			return {};
		}
		out.push_back(cp);
	}
	return out;
}

std::optional<std::string> utf8_encode(std::u32string_view str)
{
	std::string out;
	std::array<uint8_t, 5> c;
	out.reserve(str.length());
	while (str.length()) {
		auto n = encode_one(str.front(), c);
		if (n == 0) {
			assert(!"codec::utf8 encode: bad code point!");
			return {};
		}
		str.remove_prefix(1);
		out.append(c.data(), c.data() + n);
	}
	return out;
}

}
