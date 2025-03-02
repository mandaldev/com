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
#ifndef COM_SRC_MAB_SERIALIZE_HPP
#define COM_SRC_MAB_SERIALIZE_HPP
#include "../meta.hpp"
#include "abi.hpp"
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

/// (de)serialize_kvs helper
#ifndef COM_KV
#define COM_KV(s, m) #m, s.m
#endif

/// Mab: Map, Array and Binary -- a binary serialization format
namespace Com::Mab {

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
		return _size;
	}
};

template <typename T>
bool serialize(Bytes &out, T const &v);

template <typename... KVs>
bool serialize_kvs(Bytes &out, KVs&&... args);

template <typename T>
bool deserialize(BytesView in, size_t &pos, T &v);

template <typename... KVs>
bool deserialize_kvs(BytesView in, size_t &pos, KVs&&... args);

//==============================================================================
// impl
//==============================================================================

namespace detial_ {

bool serialize_basic(Bytes &out, uint8_t type, void const *data, size_t size);

bool deserialize_basic(BytesView in, size_t &pos, uint8_t type, void *data, size_t size);

// A0 Type
// An LEN Type v...
bool serialize_array_basic(Bytes &out, uint8_t element_type, size_t element_size, size_t count, void const *data);

bool deserialize_array_basic(
	BytesView in, size_t &pos,
	uint8_t element_type, size_t element_size,
	std::function<void*()> &&data,
	std::function<void(size_t)> &&resize
);

template <size_t N, typename Member, typename... Args>
bool serialize_struct_impl(Bytes &out, const char (&name)[N], Member const &member, Args&&... args)
{
	if constexpr (N == 0) {
		static_assert(N, "member name must not be empty!");
	}
	using Type = std::remove_const_t<std::remove_reference_t<Member>>;
	if constexpr (IsTemplate<Type, std::optional>::value) {
		if (member) {
			return serialize(out, std::string_view(name)) && serialize(out, member.value());
		}
		return true;
	} else {
		if (!serialize(out, std::string_view(name)) || !serialize(out, member)) {
			return false;
		}
	}
	if constexpr (sizeof...(Args)) {
		return serialize_struct_impl(out, std::forward<Args>(args)...);
	}
	return true;
}

template <size_t N, typename Member, typename... Args>
void deserialize_struct_push_members(
	std::map<std::string_view, std::function<bool(BytesView in, size_t &pos)>> &members,
	const char (&name)[N], Member &member, Args&&... args)
{
	members[name] = [&member](BytesView in, size_t &pos)
	{
		using Type = std::remove_const_t<std::remove_reference_t<Member>>;
		if constexpr (IsTemplate<Type, std::optional>::value) {
			typename Type::value_type v{};
			if (deserialize(in, pos, v)) {
				member = std::move(v);
				return true;
			}
			return false;
		} else {
			return deserialize(in, pos, member);
		}
	};
	if constexpr (sizeof...(Args)) {
		deserialize_struct_push_members(members, std::forward<Args>(args)...);
	}
}

// normal array
template <typename V>
bool serialize_array(Bytes &out, V const &v)
{
	try {
		out.push_back(array_abi);
		for (auto const &e : v) {
			if (!serialize(out, e)) {
				return false;
			}
		}
		out.push_back(abi_end);
		return true;
	} catch (...) {
		return false;
	}
}

// normal array
template <typename V>
bool deserialize_array(BytesView in, size_t &pos, V &v)
{
	using T = std::remove_reference_t<V>;
	if (in.size() >= pos + 2 && in[pos] == array_abi) {
		pos += 1;
		v.clear();
		while (in.size() > pos && in[pos] != abi_end) {
			typename T::value_type e{};
			if (!deserialize(in, pos, e)) {
				return false;
			}
			try {
				v.push_back(std::move(e));
			} catch (...) {
				return false;
			}
		}
		if (in.size() > pos) {
			pos += 1;
			return true;
		}
	}
	return false;
}

bool find_next_value_pos(BytesView in, size_t &pos);

}

/// support basic type and enum types (from abi.hpp), plus:
/// - std::vector
/// - std::string and std::string_view
/// - std::map and std::unordered_map
template <typename V>
bool serialize(Bytes &out, V const &v)
{
	using T = std::remove_const_t<std::remove_reference_t<V>>;
	if constexpr (basic_type_abi<T>() > 0) {
		return detial_::serialize_basic(out, basic_type_abi<T>(), &v, sizeof(T));
	} else if constexpr (enum_type_abi<T>() > 0) {
		return detial_::serialize_basic(out, enum_type_abi<T>(), &v, sizeof(T));
	} else if constexpr (IsTemplate<T, std::basic_string>::value || IsTemplate<T, std::basic_string_view>::value) {
		const auto abi = basic_type_abi<typename T::value_type>();
		static_assert(abi, "not supported string type!");
		return detial_::serialize_array_basic(out, abi, sizeof(typename T::value_type), v.size(), v.data());
	} else if constexpr (IsTemplate<T, std::vector>::value) {
		const auto abi = basic_type_abi<typename T::value_type>();
		if (abi) {
			return detial_::serialize_array_basic(out, abi, sizeof(typename T::value_type), v.size(), v.data());
		}
		return detial_::serialize_array(out, v);
	} else if constexpr (IsTemplate<T, std::map>::value || IsTemplate<T, std::unordered_map>::value) {
		try {
			out.push_back(map_abi);
			for (auto const &pair : v) {
				if (!serialize(out, pair.first) || !serialize(out, pair.second)) {
					return false;
				}
			}
			out.push_back(abi_end);
			return true;
		} catch (...) {
			return false;
		}
	} else if constexpr (Com::Iterable<T>::value) {
		// other containers
		// normal array
		return detial_::serialize_array(out, v);
	} else {
		static_assert(false, "not supported type!");
	}
	return false;
}

/// helper api for serializing user struct types
/// member can be std::optional type (NOTE: not supported by `serialize`)
template <typename... KVs>
bool serialize_kvs(Bytes &out, KVs&&... args)
{
	try {
		out.push_back(map_abi);
		if (detial_::serialize_struct_impl(out, std::forward<KVs>(args)...)) {
			out.push_back(abi_end);
			return true;
		}
		return false;
	} catch (...) {
		return false;
	}
}

template <typename V>
bool deserialize(BytesView in, size_t &pos, V &v)
{
	using T = std::remove_reference_t<V>;
	if constexpr (basic_type_abi<T>() > 0) {
		return detial_::deserialize_basic(in, pos, basic_type_abi<T>(), &v, sizeof(T));
	} else if constexpr (enum_type_abi<T>() > 0) {
		return detial_::deserialize_basic(in, pos, enum_type_abi<T>(), &v, sizeof(T));
	} else if constexpr (IsTemplate<T, std::basic_string>::value) {
		const auto abi = basic_type_abi<typename T::value_type>();
		static_assert(abi, "not supported string type!");
		return detial_::deserialize_array_basic(
			in, pos,
			abi, sizeof(typename T::value_type),
			[&v]() { return v.data(); },
			[&v](size_t size) { v.resize(size); }
		);
	} else if constexpr (IsTemplate<T, std::vector>::value) {
		const auto abi = basic_type_abi<typename T::value_type>();
		if (abi) {
			return detial_::deserialize_array_basic(
				in, pos,
				abi, sizeof(typename T::value_type),
				[&v]() { return v.data(); },
				[&v](size_t size) { v.resize(size); }
			);
		}
		return detial_::deserialize_array(in, pos, v);
	} else if constexpr (IsTemplate<T, std::map>::value || IsTemplate<T, std::unordered_map>::value) {
		if (in.size() >= pos + 2 && in[pos] == map_abi) {
			pos += 1;
			v.clear();
			while (in.size() > pos && in[pos] != abi_end) {
				typename T::key_type key{};
				typename T::value_type value{};
				if (!deserialize(in, pos, key) || !deserialize(in, pos, value)) {
					return false;
				}
				try {
					v[key] = std::move(value);
				} catch (...) {
					return false;
				}
			}
			if (in.size() > pos) {
				pos += 1;
			} else {
				return false;
			}
			return true;
		}
	} else if constexpr (Com::Iterable<T>::value) {
		// other containers
		// normal array
		return detial_::deserialize_array(in, pos, v);
	} else {
		static_assert(false, "not supported type!");
	}
	return false;
}

template <typename V>
bool deserialize(BytesView in, V &v)
{
	size_t pos = 0;
	return deserialize(in, pos, v);
}

template <typename... KVs>
bool deserialize_kvs(BytesView in, size_t &pos, KVs&&... args)
{
	if (in.size() < pos + 2 || in[pos] != map_abi) {
		return false;
	}
	pos += 1;
	std::map<std::string_view, std::function<bool(BytesView in, size_t &pos)>> members;
	detial_::deserialize_struct_push_members(members, std::forward<KVs>(args)...);
	while (in.size() > pos && in[pos] != abi_end) {
		std::string key;
		if (!deserialize(in, pos, key)) {
			return false;
		}
		auto it = members.find(key);
		if (it != members.end()) {
			if (!it->second(in, pos)) {
				return false;
			}
		} else if (!detial_::find_next_value_pos(in, pos)) { // drop it
			return false;
		}
	}
	if (in.size() <= pos) {
		return false;
	}
	pos += 1;
	return true;
}

template <typename... KVs>
bool deserialize_kvs(BytesView in, KVs&&... args)
{
	size_t pos = 0;
	return deserialize_kvs(in, pos, std::forward<KVs>(args)...);
}

}
#endif
