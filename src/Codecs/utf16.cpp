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
#include "utf16.hpp"
#include <cstdint>
#include <string>
#include <string_view>

namespace Com {

std::optional<std::u16string> utf16_encode(std::u32string_view str)
{
	/// Encoding of a single character from an ISO 10646 character value to
	/// UTF-16 proceeds as follows. Let U be the character number, no greater
	/// than 0x10FFFF.
	/// 1) If U < 0x10000, encode U as a 16-bit unsigned integer and
	///    terminate.
	/// 2) Let U' = U - 0x10000. Because U is less than or equal to 0x10FFFF,
	///    U' must be less than or equal to 0xFFFFF. That is, U' can be
	///    represented in 20 bits.
	/// 3) Initialize two 16-bit unsigned integers, W1 and W2, to 0xD800 and
	///    0xDC00, respectively. These integers each have 10 bits free to
	///    encode the character value, for a total of 20 bits.
	/// 4) Assign the 10 high-order bits of the 20-bit U' to the 10 low-order
	///    bits of W1 and the 10 low-order bits of U' to the 10 low-order
	///    bits of W2. Terminate.
	///
	/// Graphically, steps 2 through 4 look like:
	/// U' = yyyyyyyyyyxxxxxxxxxx
	/// W1 = 110110yyyyyyyyyy
	/// W2 = 110111xxxxxxxxxx

	std::u16string ret;
	ret.reserve(str.size() * 3);
	for (uint32_t c : str) {
		if (c > 0x10FFFF) {
			return {};
		}
		if (c < 0x10000) {
			ret.push_back(char16_t(c));
			continue;
		}
		auto u = c - 0x10000;
		uint16_t w1 = 0xD800, w2 = 0xDC00;
		w1 |= 0x3FF & (u >> 10);
		w2 |= 0x3FF & u;
		ret.push_back(w1);
		ret.push_back(w2);
	}

	return ret;
}

std::optional<std::u32string> utf16_decode(void const *buf, size_t len, bool little_endian)
{
	if (len % 2) {
		return {};
	}
	// linux is little endian
	if (little_endian) {
		auto str = std::u16string_view((char16_t*)buf, len / 2);
		return utf16_decode(str);
	}
	std::u16string str(len / 2, 0);
	for (size_t i = 0, j = 0; i < str.size(); i++) {
		uint16_t h = ((uint8_t*)buf)[j++];
		uint16_t l = ((uint8_t*)buf)[j++];
		str[i] = (h << 16) | l;
	}
	return utf16_decode(str);
}

std::optional<std::u32string> utf16_decode(std::u16string_view str)
{
	// Decoding of a single character from UTF-16 to an ISO 10646 character
	// value proceeds as follows. Let W1 be the next 16-bit integer in the
	// sequence of integers representing the text. Let W2 be the (eventual)
	// next integer following W1.
	// 1) If W1 < 0xD800 or W1 > 0xDFFF, the character value U is the value
	//    of W1. Terminate.
	// 2) Determine if W1 is between 0xD800 and 0xDBFF. If not, the sequence
	//    is in error and no valid character can be obtained using W1.
	//    Terminate.
	// 3) If there is no W2 (that is, the sequence ends with W1), or if W2
	//    is not between 0xDC00 and 0xDFFF, the sequence is in error.
	//    Terminate.
	// 4) Construct a 20-bit unsigned integer U', taking the 10 low-order
	//    bits of W1 as its 10 high-order bits and the 10 low-order bits of
	//    W2 as its 10 low-order bits.
	// 5) Add 0x10000 to U' to obtain the character value U. Terminate.
	std::u32string ret;
	for (size_t i = 0; i < str.size(); i++) {
		uint32_t w1 = str[i];
		if (w1 < 0xD800 || w1 > 0xDFFF) {
			ret.push_back(w1);
			continue;
		}
		if (w1 > 0xDBFF) {
			return {};
		}
		auto j = i + 1;
		if (j >= str.size()) {
			return {};
		}
		uint32_t w2 = str[j];
		if (w2 < 0xDC00 || w2 > 0xDFFF) {
			return {};
		}
		ret.push_back(((0x3FF & w1) << 10) | (0x3FF & w2));
	}
	return ret;
}

}
