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
#include "serialize.hpp"
#include "abi.hpp"
#include <cstdint>
#include <cstring>
#include <functional>

namespace Com::Mab::detial_ {

static inline uint8_t length_size(size_t len)
{
	for (int i = 7; i >= 0; i--) {
		if (((uint8_t*)&len)[i]) {
			return i + 1;
		}
	}
	return 0;
}

bool serialize_basic(Bytes &out, uint8_t type, void const *data, size_t size)
try {
	// _buf.reserve(_buf.size() + 1 + size);
	out.push_back(type);
	auto p = (uint8_t const*)data;
	out.insert(out.end(), p, p + size);
	return true;
} catch (...) {
	return false;
}

bool serialize_array_basic(Bytes &out, uint8_t element_type, size_t element_size, size_t count, void const *data)
try {
	auto len_size = length_size(count);
	out.push_back(array_0_abi + len_size);
	if (count) {
		// NOTE little endian
		auto p = (uint8_t*)&count;
		out.insert(out.end(), p, p + len_size);
	}
	out.push_back(element_type);
	auto p = (uint8_t const*)data;
	out.insert(out.end(), p, p + element_size * count);
	return true;
} catch (...) {
	return false;
}

bool deserialize_basic(BytesView in, size_t &pos, uint8_t type, void *data, size_t size)
{
	if (in.size() < pos + 1 + size) {
		return false;
	}
	if (in[pos++] != type) {
		return false;
	}
	std::memcpy(data, in.data() + pos, size);
	pos += size;
	return true;
}

bool deserialize_array_basic(
	BytesView in, size_t &pos,
	uint8_t element_type, size_t element_size,
	std::function<void*()> &&data,
	std::function<void(size_t)> &&resize
)
{
	if (in.size() < pos + 2) {
		return false;
	}
	auto array_n = in[pos++];
	if (array_n < array_0_abi || array_n > array_8_abi) {
		return false;
	}
	auto len_size = array_n - array_0_abi;
	size_t count = 0;
	if (in.size() < pos + len_size) {
		return false;
	}
	memcpy(&count, in.data() + pos, len_size);
	pos += len_size;
	if (in.size() < pos + 1 + count) {
		return false;
	}
	if (in[pos++] != element_type) {
		return false;
	}
	try {
		resize(count);
		auto size = element_size * count;
		memcpy(data(), in.data() + pos, size);
		pos += size;
		return true;
	} catch (...) {
		return false;
	}
}

bool find_next_value_pos(BytesView in, size_t &pos)
{
	if (in.size() <= pos) {
		return false;
	}
	auto abi = in[pos++];
	auto size = scalar_type_size(abi);
	if (size) {
		pos += size;
		return pos <= in.size();
	}
	if (abi >= array_0_abi && abi <= array_8_abi) {
		size_t len = 0;
		auto len_size = abi - array_0_abi;
		if (pos + len_size > in.size()) {
			return false;
		}
		memcpy(&len, in.data() + pos, len_size);
		pos += len_size;
		if (pos >= in.size()) {
			return false;
		}
		auto e_abi = in[pos++];
		auto e_size = scalar_type_size(e_abi);
		if (e_size == 0) {
			return false;
		}
		pos += e_size * len;
		return true;
	}
	switch (abi) {
	case array_abi:
		while (pos < in.size() && in[pos] != abi_end) {
			if (!find_next_value_pos(in, pos)) {
				return false;
			}
		}
		if (pos < in.size() && in[pos] == abi_end) {
			pos += 1;
			return true;
		}
		break;
	case map_abi:
		while (pos < in.size() && in[pos] != abi_end) {
			if (!find_next_value_pos(in, pos) || !find_next_value_pos(in, pos)) {
				return false;
			}
		}
		if (pos < in.size() && in[pos] == abi_end) {
			pos += 1;
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

}
