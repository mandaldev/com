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
#ifndef CGEN_SRC_MAS_SERIALIZE_HPP_
#define CGEN_SRC_MAS_SERIALIZE_HPP_
#include "../Codecs/utf8.hpp"
#include "../meta.hpp"
#include "Value.hpp"
#include <array>
#include <charconv>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <variant>

/// Serialize API
///
/// It's convenient if we have api to serialize C++ data to Mas,
/// this file provides template functions to do that.

namespace Com::Mas {

template <typename T>
Mas::Value serialize(T const &v);

template <typename T>
bool deserialize(Mas::Value const &mas, T &v);

/// helper of serialize key-value pairs (struct, class...) to mas (Mas::Map), for example
/// ```
/// struct A {
/// 	int value1;
/// 	int value2;
/// };
/// A a;
/// auto mas = serialize_kvs("value1", a.value1, "value2", a.value2);
/// ```
template <typename... KVs>
Mas::Value serialize_kvs(KVs&&... args);

/// helper of deserialize mas (Mas::Map) to key-value pairs (struct, class...), for example
/// ```
/// struct A {
/// 	int value1;
/// 	int value2;
/// };
/// A a;
/// Mas::Value mas;
/// auto succeed = deserialize_kvs(mas, "value1", a.value1, "value2", a.value2);
/// ```
template <typename... KVs>
bool deserialize_kvs(Mas::Value const &mas, KVs&&... args);

/// (de)serialize_kvs helper
#define COM_KV(s, m) #m, s.m

/// helper for serializing enums
/// never put this inside any namesapces
/// usage: `COM_E(EnumType, enum1, enum2, ...)`
#define COM_ENUM(enum_type, ...) \
namespace Com::Mas \
{ \
template <> \
Value serialize<enum_type>(enum_type const &value) \
{ \
	auto &EN = enum_to_names<enum_type, __VA_ARGS__>(); \
	auto it = EN.find(value); \
	if (it != EN.end()) { \
		return std::string(it->second); \
	} \
	return std::string(); \
} \
template <> \
bool deserialize<enum_type>(Value const &mas, enum_type &v) \
{ \
	if (!mas.is_string()) { \
		return false; \
	} \
	auto &NE = name_to_enum<enum_type, __VA_ARGS__>(); \
	auto it = NE.find(mas.string()); \
	if (it != NE.end()) { \
		v = it->second; \
		return true; \
	} \
	return false; \
} \
}

/// signatures of `COM_ENUM` only
#define COM_ENUM_DECL(enum_type) \
namespace Com::Mas \
{ \
template <> Value serialize<enum_type>(enum_type const &value); \
template <> bool deserialize<enum_type>(Value const &mas, enum_type &v); \
}

// impl

namespace detial_ {

extern bool Deserialize_Of_This_Type_Not_Implemented;
extern Mas::Value Serialize_Of_This_Type_Not_Implemented;

/// test `T` can be inserted with one item
template <typename T, typename... Args>
Mas::Value number_to_mas(T v, Args&&... args)
{
	std::array<char, 64> str;
	auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), v, args...);
	if (ec == std::errc()) {
        	return Mas::String(str.data(), ptr);
	} else {
 	       return Mas::String();
	}
}

template <typename T>
bool number_from_str(T &v, Mas::String const & str)
{
	auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), v);
	if (ec == std::errc()) {
        	return true;
	}
 	return false;
}

template <typename T>
bool number_from_mas(T &v, Mas::Value const & mas)
{
	return mas.is_string() && number_from_str(v, mas.string());
}

template <size_t N, typename Member, typename... Args>
void struct_to_mas_impl(Mas::Map &mas, const char (&name)[N], Member const &member, Args&&... args)
{
	mas.insert({Mas::String(name), Com::Mas::serialize(member)});
	if constexpr (sizeof...(Args)) {
		struct_to_mas_impl(mas, std::forward<Args>(args)...);
	}
}

template <size_t N, typename Member, typename... Args>
void struct_from_mas_impl(Mas::Map const &mas, const char (&name)[N], Member &member, Args&&... args)
{
	if (auto it = mas.find(name); it != mas.end()) {
		Com::Mas::deserialize(it->second, member);
	}
	if constexpr (sizeof...(Args)) {
		struct_from_mas_impl(mas, std::forward<Args>(args)...);
	}
}

template <typename Sequence, typename Pusher>
bool sequence_from_mas(Sequence &c, Mas::Value const &mas, Pusher &&push)
{
	c.clear();
	if (!mas.is_array()) {
		return false;
	}
	for (auto &item : mas.array()) {
		if constexpr (Com::IsTemplate<Sequence, std::map>::value
		|| Com::IsTemplate<Sequence, std::unordered_map>::value
		|| Com::IsTemplate<Sequence, std::multimap>::value
		|| Com::IsTemplate<Sequence, std::unordered_multimap>::value
		) {
			std::pair<typename Sequence::key_type, typename Sequence::mapped_type> v;
			if (Com::Mas::deserialize(item, v)) {
				push(c, std::move(v));
			}
		} else { // template of set then
			typename Sequence::value_type v;
			if (Com::Mas::deserialize(item, v)) {
				push(c, std::move(v));
			}
		}
	}
	return true;
}

template <size_t I, typename T>
bool deserialize_variant(Mas::Value const &mas, T &v)
{
	std::variant_alternative_t<I, T> result;
	if (deserialize(mas, result)) {
		v = std::move(result);
		return true;
	}
	return false;
}

}

template <typename V>
Mas::Value serialize(V const &v)
{
	using T = std::remove_const_t<std::remove_reference_t<V>>;
	if constexpr (std::is_same_v<T, bool>) {
		return v ? "true" : "false";
	} else if constexpr (std::is_same_v<T, char>) {
		return std::string(1, v);
	} else if constexpr (std::is_same_v<T, char32_t>) {
		return utf8_encode(std::u32string(1, v)).value_or("\0");
	} else if constexpr (std::is_integral_v<T>) {
		return detial_::number_to_mas(v, 10);
	} else if constexpr (std::is_floating_point_v<T>) {
		return detial_::number_to_mas(v, std::chars_format::scientific, 32);
	} else if constexpr (std::is_same_v<T, std::string>) {
		return v;
	} else if constexpr (std::is_same_v<T, std::u32string>) {
		// encode to utf-8
		return Com::utf8_encode(v).value_or("");
	} else if constexpr (std::is_same_v<T, std::filesystem::path>) {
		// to string
		return v.string();
	} else if constexpr (Com::IsTemplate<T, std::pair>::value) {
		return serialize_kvs(COM_KV(v, first), COM_KV(v, second));
	} else if constexpr (
		Com::IS_MAP_WITH<T, std::string>
		|| Com::IS_MAP_WITH<T, std::u32string>
		|| Com::IS_MAP_WITH<T, char>
		|| Com::IS_MAP_WITH<T, char32_t>
	) {
		// string keyed maps, use Mas::map directly
		// encode key to utf-8
		Mas::Map m;
		for (auto &p : v) {
			m.emplace(serialize(p.first).as_string(), serialize(p.second));
		}
		return m;
	} else if constexpr (Com::IsTemplate<T, std::variant>::value) {
		return std::visit([](auto &o)
		{
			return serialize(o);
		}, v);
	} else if constexpr (Com::Iterable<T>::value) {
		// other containers
		// use Mas::Array
		Mas::Array a;
		for (auto &item : v) {
			a.push_back(serialize(item));
		}
		return a;
	}
	return detial_::Serialize_Of_This_Type_Not_Implemented;
}

/// from string
template <typename T>
bool deserialize(Mas::String const &str, T &v)
{
	if constexpr (std::is_same_v<T, bool>) {
		if (str == "true") {
			v = true;
		} else if (str == "false") {
			v = false;
		} else {
			return false;
		}
		return true;
	} else if constexpr (std::is_same_v<T, char>) {
		if (str.size() != 1) {
			return false;
		}
		v = str[0];
		return true;
	} else if constexpr (std::is_same_v<T, char32_t>) {
		auto us = utf8_decode(str);
		if (!us) {
			return false;
		}
		if (us.value().size() != 1) {
			return false;
		}
		v = us.value()[0];
		return true;
	} else if constexpr (std::is_arithmetic_v<T>) {
		return detial_::number_from_str(v, str);
	} else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::filesystem::path>) {
		v = str;
		return true;
	} else if constexpr (std::is_same_v<T, std::u32string>) {
		// encoded to utf-8
		auto res = Com::utf8_decode(str);
		if (res.has_value()) {
			v = res.value();
			return true;
		}
		return false;
	}
	return detial_::Deserialize_Of_This_Type_Not_Implemented;
}

template <typename T>
bool deserialize_map(Mas::Value const &mas, T &v)
{
	// string keyed maps, use Mas::map directly
	if (!mas.is_map()) {
		return false;
	}
	v.clear();
	for (auto &p : mas.map()) {
		typename T::key_type k;
		if (deserialize(p.first, k)) {
			typename T::mapped_type item;
			if (deserialize(p.second, item)) {
				v.emplace(std::move(k), std::move(item));
			}
		}
	}
	return true;
}

template <typename T>
bool deserialize(Mas::Value const &mas, T &v)
{
	if constexpr (
		std::is_same_v<T, bool>
		|| std::is_same_v<T, char>
		|| std::is_same_v<T, char32_t>
		|| std::is_arithmetic_v<T>
		|| std::is_same_v<T, std::string>
		|| std::is_same_v<T, std::u32string>
		|| std::is_same_v<T, std::filesystem::path>
	) {
		return mas.is_string() && deserialize(mas.string(), v);
	} else if constexpr (Com::IsTemplate<T, std::pair>::value) {
		return deserialize_kvs(mas, COM_KV(v, first), COM_KV(v, second));
	} else if constexpr (
		Com::IS_MAP_WITH<T, bool>
		|| Com::IS_MAP_WITH<T, char>
		|| Com::IS_MAP_WITH<T, char32_t>
		|| Com::IsArithmeticKeyedMap<T>::value
		|| Com::IS_MAP_WITH<T, std::string>
		|| Com::IS_MAP_WITH<T, std::u32string>
		|| Com::IS_MAP_WITH<T, std::filesystem::path>
	) {
		return deserialize_map(mas, v);
	} else if constexpr (Com::IsTemplate<T, std::variant>::value) {
		if constexpr (0 < std::variant_size_v<T>) {
			if (detial_::deserialize_variant<0>(mas, v)) {
				return true;
			}
		}
		if constexpr (1 < std::variant_size_v<T>) {
			if (detial_::deserialize_variant<1>(mas, v)) {
				return true;
			}
		}
		if constexpr (2 < std::variant_size_v<T>) {
			if (detial_::deserialize_variant<2>(mas, v)) {
				return true;
			}
		}
		return false;
	} else if constexpr (Insertable<T>::value) {
		// other containers
		// use Mas::Array
		return detial_::sequence_from_mas(v, mas, [](T& seq, auto&& item) { seq.insert(std::forward<decltype(item)>(item)); });
	} else if constexpr (Pushbackable<T>::value) {
		// other containers
		// use Mas::Array
		return detial_::sequence_from_mas(v, mas, [](T& seq, auto&& item) { seq.push_back(std::forward<decltype(item)>(item)); });
	}
	return detial_::Deserialize_Of_This_Type_Not_Implemented;
}

template <typename... KVs>
Mas::Value serialize_kvs(KVs&&... args)
{
	Mas::Value mas;
	auto &map = mas.as_map();
	detial_::struct_to_mas_impl(map, std::forward<KVs>(args)...);
	return mas;
}

template <typename... KVs>
bool deserialize_kvs(Mas::Value const &mas, KVs&&... args)
{
	if (!mas.is_map()) {
		return false;
	}
	auto &map = mas.map();
	detial_::struct_from_mas_impl(map, std::forward<KVs>(args)...);
	return true;
}

}
#endif
