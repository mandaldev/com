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
#include "../../src/Codecs/base16.hpp"
#include <string>
#include <cassert>

TEST(codecs_base16, "Com Codecs base16 test")
{
	std::string buf;
	buf.resize(256);
	for (int i = 0; i < 256; i++) {
		buf[i] = i;
	}
	auto base16 = Com::base16_encode(buf.data(), buf.size());
	auto decoded = Com::base16_decode(base16);
	assert(decoded.has_value());
	assert(decoded.value() == buf);
}
