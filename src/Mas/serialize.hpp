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
#ifndef _CGEN_SRC_MAS_SERIALIZE_HPP_
#define _CGEN_SRC_MAS_SERIALIZE_HPP_
#include "Value.hpp"
#include "../meta.hpp"
#include "../Codecs/utf8.hpp"
#include <functional>
#include <charconv>
#include <filesystem>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

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

// impl

namespace __detial {

extern bool Deserialize_Of_This_Type_Not_Implemented;
extern Mas::Value Serialize_Of_This_Type_Not_Implemented;

/// test `T` can be inserted with one item
template <typename T, typename = void>
struct Insertable
: std::false_type
{};

template <typename T>
struct Insertable<T, std::enable_if_t<true, decltype(std::declval<std::remove_reference_t<T>>().insert({}))>>
: std::true_type
{};

static_assert(Insertable<std::map<int, int>>::value);
static_assert(!Insertable<const std::map<int, int>>::value);

/// test `T` can be push back with one item
template <typename T, typename = void>
struct Pushbackable
: std::false_type
{};

template <typename T>
struct Pushbackable<T, std::enable_if_t<true, decltype(std::declval<std::remove_reference_t<T>>().push_back({}))>>
: std::true_type
{};

static_assert(Pushbackable<std::vector<int>>::value);
static_assert(!Pushbackable<const std::vector<int>>::value);

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
bool number_from_mas(T &v, Mas::Value const & mas)
{
	auto str = mas.string_or("");
	auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), v);
	if (ec == std::errc()) {
        	return true;
	}
 	return false;
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
	for (auto &item : mas.array_or({})) {
		typename Sequence::value_type v;
		if (Com::Mas::deserialize(item, v)) {
			push(c, std::move(v));
		}
	}
	return true;
}

}

template <typename T>
Mas::Value serialize(T const &v)
{
	if constexpr (std::is_same_v<T, bool>) {
		return v ? "true" : "false";
	} else if constexpr (std::is_integral_v<T>) {
		return __detial::number_to_mas(v, 10);
	} else if constexpr (std::is_floating_point_v<T>) {
		return __detial::number_to_mas(v, std::chars_format::scientific, 32);
	} else if constexpr (std::is_same_v<T, std::string>) {
		return v;
	} else if constexpr (std::is_same_v<T, std::u32string>) {
		// encode to utf-8
		return Com::utf8_encode(v).value_or("");
	} else if constexpr (std::is_same_v<T, std::filesystem::path>) {
		// to string
		return v.string();
	} else if constexpr (Com::IsTemplate<T, std::pair>::value) {
		return serialize_kvs(COM_KV(v, frist), COM_KV(v, second));
	} else if constexpr (Com::IsTemplateWith<T, std::map, std::string>::value
		|| Com::IsTemplateWith<T, std::unordered_map, std::string>::value) {
		// string keyed maps, use Mas::map directly
		Mas::Map m;
		for (auto &p : v) {
			m.emplace(p.first, serialize(p.second));
		}
		return m;
	} else if constexpr (Com::IsTemplateWith<T, std::map, std::u32string>::value
		|| Com::IsTemplateWith<T, std::unordered_map, std::u32string>::value) {
		// string keyed maps, use Mas::map directly
		// encode key to utf-8
		Mas::Map m;
		for (auto &p : v) {
			m.emplace(Com::utf8_encode(p.first).value_or(""), serialize(p.second));
		}
		return m;
	} else if constexpr (Com::Iterable<T>::value) {
		// other containers
		// use Mas::Array
		Mas::Array a;
		for (auto &item : v) {
			a.push_back(serialize(item));
		}
		return a;
	} else {
		return __detial::Serialize_Of_This_Type_Not_Implemented;
	}
}

template <typename T>
bool deserialize(Mas::Value const &mas, T &v)
{
	if constexpr (std::is_same_v<T, bool>) {
		auto str = mas.string_or("");
		if (str == "true") {
			v = true;
		} else if (str == "false") {
			v = false;
		} else {
			return false;
		}
		return true;
	} else if constexpr (std::is_arithmetic_v<T>) {
		return __detial::number_from_mas(v, mas);
	} else if constexpr (std::is_same_v<T, std::string>) {
		if (mas.is_string()) {
			v = mas.string_or("");
			return true;
		}
		return false;
	} else if constexpr (std::is_same_v<T, std::u32string>) {
		// encoded to utf-8
		if (mas.is_string()) {
			auto res = Com::utf8_decode(mas.string_or(""));
			if (res.has_value()) {
				v = res.value();
				return true;
			}
		}
		return false;
	} else if constexpr (std::is_same_v<T, std::filesystem::path>) {
		// as string
		if (mas.is_string()) {
			v = mas.string_or("");
			return true;
		}
		return false;
	} else if constexpr (Com::IsTemplate<T, std::pair>::value) {
		return deserialize_kvs(mas, COM_KV(v, first), COM_KV(v, second));
	} else if constexpr (Com::IsTemplateWith<T, std::map, std::string>::value
		|| Com::IsTemplateWith<T, std::unordered_map, std::string>::value) {
		// string keyed maps, use Mas::map directly
		if (!mas.is_map()) {
			return false;
		}
		v.clear();
		for (auto &p : mas.map_or({})) {
			typename T::mapped_type item;
			if (deserialize(p.second, item)) {
				v.insert({p.first, std::move(item)});
			}
		}
		return true;
	} else if constexpr (Com::IsTemplateWith<T, std::map, std::u32string>::value
		|| Com::IsTemplateWith<T, std::unordered_map, std::u32string>::value) {
		// string keyed maps, use Mas::map directly
		// encode key to utf-8
		if (!mas.is_map()) {
			return false;
		}
		v.clear();
		for (auto &p : mas.map_or({})) {
			auto key = Com::utf8_decode(p.first).value_or(U"");
			if (key.empty()) {
				continue;
			}
			typename T::mapped_type item;
			if (deserialize(p.second, item)) {
				v.insert({key, std::move(item)});
			}
		}
		return true;
	} else if constexpr (__detial::Insertable<T>::value) {
		// other containers
		// use Mas::Array
		return __detial::sequence_from_mas(v, mas, [](T &seq, auto &&item) { seq.insert(std::move(item)); });
	} else if constexpr (__detial::Pushbackable<T>::value) {
		// other containers
		// use Mas::Array
		return __detial::sequence_from_mas(v, mas, [](T &seq, auto &&item) { seq.push_back(std::move(item)); });
	} else {
		return __detial::Deserialize_Of_This_Type_Not_Implemented;
	}
}

template <typename... KVs>
Mas::Value serialize_kvs(KVs&&... args)
{
	Mas::Value mas;
	auto &map = mas.as_map();
	__detial::struct_to_mas_impl(map, std::forward<KVs>(args)...);
	return mas;
}

template <typename... KVs>
bool deserialize_kvs(Mas::Value const &mas, KVs&&... args)
{
	if (!mas.is_map()) {
		return false;
	}
	auto &map = mas.map_or({});
	__detial::struct_from_mas_impl(map, std::forward<KVs>(args)...);
	return true;
}

}
#endif
