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
#include "../../src/unit.hpp"
#include "../../src/Codecs/utf16.hpp"
#include <cassert>
#include <string>

TEST(codecs_utf16, "Com Codecs utf16 test")
{
	std::u32string buf;
	buf.reserve((size_t)4 * 128);
	for (auto bits : { 0, 7, 11 }) {
		int c = 1 << bits;
		for (int i = 0; i < 128; i++) {
			buf.push_back(c++);
		}
	}
	auto utf16 = Com::utf16_encode(buf);
	assert(utf16);
	auto decoded = Com::utf16_decode(utf16.value());
	assert(decoded.has_value());
	assert(decoded.value() == buf);
}
