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
#include "base16.hpp"

namespace Com {

static const char HEX_CHARS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', };

std::string base16_encode(void const *buf, size_t size)
{
	std::string out;
	out.resize(size * 2);
	base16_encode(buf, size, out.data());
	return out;
}

void base16_encode(void const *in_buf, size_t in_size, char *out_buf)
{
	auto in = (uint8_t const *)in_buf;
	auto in_end = in + in_size;
	while (in != in_end) {
		auto byte = *in++;
		*out_buf++ = HEX_CHARS[byte >> 4];
		*out_buf++ = HEX_CHARS[byte & 0x0f];
	}
}

std::optional<std::string> base16_decode(std::string const &buf)
{
	std::string out;
	out.resize(buf.size() / 2);
	if (base16_decode(buf.data(), buf.size(), out.data())) {
		return std::move(out);
	}
	return {};
}

bool base16_decode(char const *in_buf, size_t in_size, void *out_buf)
{
	if (in_size % 2) {
		return false;
	}
	auto in_end = in_buf + in_size;
	auto out = (uint8_t *)out_buf;
	while (in_buf != in_end) {
		auto h1 = *in_buf++;
		auto h2 = *in_buf++;
		for (auto p : { &h1, &h2, }) {
			auto &h = *p;
			if (h >= '0' && h <= '9') {
				h -= '0';
			} else if (h >= 'a' && h <= 'f') {
				h -= 'a' - 10;
			} else if (h >= 'A' && h <= 'F') {
				h -= 'A' - 10;
			} else {
				return false;
			}
		}
		*out++ = (h1 << 4) + h2;
	}
	return true;
}

}
