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
#include "../../src/Mas/serialize.hpp"
#include <string>
#include <cassert>

struct A {
	int i = 0;
	bool b = false;
	float f = 1.0f;
};
bool operator==(A const &l, A const &r)
{
	return l.i == r.i && l.b == r.b && l.f == r.f;
}
namespace Com::Mas {
template <>
Value serialize(A const &v)
{
	return serialize_kvs(COM_KV(v, i), COM_KV(v, b), COM_KV(v, f));
}
template <>
bool deserialize(Value const &mas, A &v)
{
	return deserialize_kvs(mas, COM_KV(v, i), COM_KV(v, b), COM_KV(v, f));
}
}

TEST(mas_serialize, "Com Mas serializing test")
{
	{
		std::string v("key"), v_;
		assert(Com::Mas::deserialize(Com::Mas::serialize(v), v_) && v_ == v);
	}
	{
		std::vector<int> v = {1, 2, 3}, v_;
		assert(Com::Mas::deserialize(Com::Mas::serialize(v), v_) && v_ == v);
	}
	{
		std::vector<std::vector<float>> v = { { 1.0f, }, { 2.0f, }, }, v_;
		assert(Com::Mas::deserialize(Com::Mas::serialize(v), v_) && v_ == v);
	}
	{
		std::map<std::string, bool> v = { { "1",  true, }, {  "2", false, }, }, v_;
		assert(Com::Mas::deserialize(Com::Mas::serialize(v), v_) && v_ == v);
	}

	{
		A v = { 1, true, 2.0f, }, v_;
		assert(Com::Mas::deserialize(Com::Mas::serialize(v), v_) && v_ == v);
	}

	// struct A {};
	// Com::Mas::serialize(A());
}
