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
#ifndef COM_SRC_BYTESVIEW_HPP
#define COM_SRC_BYTESVIEW_HPP
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace Com {

using Bytes = std::vector<uint8_t>;

struct BytesView
{
	uint8_t const *_data = nullptr;
	size_t _size = 0;

	BytesView(Bytes const &bytes)
	: _data(bytes.data())
	, _size(bytes.size())
	{}

	BytesView(std::string_view str)
	: _data((uint8_t const *)str.data())
	, _size(str.size())
	{}

	BytesView(uint8_t const *bytes, size_t count)
	: _data(bytes)
	, _size(count)
	{
	}

	BytesView(Bytes const &bytes, size_t pos, size_t count)
	: _data(bytes.data() + (std::ptrdiff_t)pos)
	, _size(count)
	{
	}

	BytesView(Bytes::const_iterator begin, Bytes::const_iterator end)
	: _data(begin.base())
	, _size(end - begin)
	{
	}

	uint8_t const &operator[](size_t index) const
	{
		return _data[index];
	}

	uint8_t const *data() const
	{
		return _data;
	}

	size_t size() const
	{
		return _size;
	}

	bool empty() const
	{
		return !_size;
	}

	BytesView subview(size_t pos, size_t size = size_t(-1))
	{
		assert(_size >= size + pos);
		return BytesView(_data + pos, std::min(_size - pos, size));
	}

	uint8_t const *begin() { return _data; }
	uint8_t const *end() { return _data + _size; }
};

}
#endif
