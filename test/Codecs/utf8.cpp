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
#include "../../src/unit.hpp"
#include "../../src/Codecs/utf8.hpp"
#include <string>
#include <cassert>

TEST(codecs_utf8, "Com Codecs utf8 test")
{
	std::u32string buf;
	buf.reserve(4 * 128);
	for (auto bits : { 0, 7, 11, 16 }) {
		int c = 1 << bits;
		for (int i = 0; i < 128; i++) {
			buf.push_back(c++);
		}
	}
	auto utf8 = Com::Codecs::utf8_encode(buf);
	assert(utf8);
	auto decoded = Com::Codecs::utf8_decode(utf8.value());
	assert(decoded.has_value());
	assert(decoded.value() == buf);
}
