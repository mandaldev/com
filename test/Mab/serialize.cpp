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
#include "../../src/unit.hpp"
#include "../../src/Mab/serialize.hpp"
#include <cstdint>
#include <optional>
#include <vector>

TEST(mab_serialize, "Com Mab serializing test")
{
	using namespace Com;
	using namespace Com::Mab;

	enum class ei8 : int8_t  { a, b };
	enum class eu8 : uint8_t  { a, b };
	enum class ei16 : int16_t  { a, b };
	enum class eu16 : uint16_t  { a, b };
	enum class ei32 : int32_t  { a, b };
	enum class eu32 : uint32_t  { a, b };
	enum class ei64 : int64_t  { a, b };
	enum class eu64 : uint64_t  { a, b };

	{
		Bytes bytes;
		size_t pos = 0;
		ei8 v = ei8::b, _v = ei8::a;
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}
	{
		Bytes bytes;
		size_t pos = 0;
		eu8 v = eu8::b, _v = eu8::a;
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}
	{
		Bytes bytes;
		size_t pos = 0;
		ei32 v = ei32::b, _v = ei32::a;
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}
	{
		Bytes bytes;
		size_t pos = 0;
		eu64 v = eu64::b, _v = eu64::a;
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}
	{
		Bytes bytes;
		size_t pos = 0;
		std::vector<int> v = {1, 2, 3}, _v;
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}
	{
		Bytes bytes;
		size_t pos = 0;
		std::vector<int> v, _v = {1, 2, 3};
		assert(serialize(bytes, v) && deserialize(bytes, pos, _v) && v == _v);
		assert(pos == bytes.size());
	}

	struct A {
		ei8 e1 = ei8::b;
		int i32 = -25;
		uint64_t u64 = 0xffff;
		std::vector<short> i16s = {1, -2, 0};
		std::optional<float> opt = 0.375f;
		std::u32string ustr = U"中文";
	};
	{
		A a, b;
		b.e1 = ei8::a;
		b.i32 = 0;
		b.u64 = 0;
		b.i16s = {};
		b.ustr = U"";
		Bytes bytes;
		size_t pos = 0;
		assert(
			serialize_kvs(bytes, COM_KV(a, e1), COM_KV(a, i32), COM_KV(a, u64), COM_KV(a, i16s), COM_KV(a, opt), COM_KV(a, ustr))
			&& deserialize_kvs(bytes, pos, COM_KV(b, e1), COM_KV(b, i32), COM_KV(b, u64), COM_KV(b, i16s), COM_KV(b, opt), COM_KV(b, ustr))
			&& a.e1 == b.e1
			&& a.i32 == b.i32
			&& a.u64 == b.u64
			&& a.i16s == b.i16s
			&& a.opt == b.opt
		);
		assert(pos == bytes.size());
	}
}
