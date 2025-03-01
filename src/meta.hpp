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
#ifndef COM_SRC_META_HPP_
#define COM_SRC_META_HPP_
#include <iterator>
#include <map>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Com {

// hide details
namespace detial_ {

template <typename... Args>
struct Test1
{};

template <typename... Args>
struct Test2
{};

template <template <typename...> typename Template, typename... Args>
int take_template(Template<Args...>);

template <typename T, template <typename...> typename Template, typename... Args>
int take_template_with(Template<T, Args...>);

}

/// test `T` is a template type `Template<...>`
template <typename T, template <typename...> typename Template, typename = void>
struct IsTemplate
: std::false_type
{};

template <typename T, template <typename...> typename Template>
struct IsTemplate<T, Template, std::void_t<decltype(detial_::take_template<Template>(std::declval<T>()))>>
: std::true_type
{};

// test
static_assert(IsTemplate<detial_::Test1<int>, detial_::Test1>::value);
static_assert(!IsTemplate<detial_::Test1<int>, detial_::Test2>::value);

template <typename T>
inline constexpr bool IS_MAP = Com::IsTemplate<T, std::map>::value
|| Com::IsTemplate<T, std::multimap>::value
|| Com::IsTemplate<T, std::unordered_map>::value
|| Com::IsTemplate<T, std::unordered_multimap>::value;

template <typename T>
inline constexpr bool IS_SET = Com::IsTemplate<T, std::set>::value
|| Com::IsTemplate<T, std::multiset>::value
|| Com::IsTemplate<T, std::unordered_set>::value
|| Com::IsTemplate<T, std::unordered_multiset>::value;

/// test `T` is a template type `Template<First, ...>`
template <typename T, template <typename...> typename Template, typename First, typename = void>
struct IsTemplateWith
: std::false_type
{};

template <typename T, template <typename...> typename Template, typename First>
struct IsTemplateWith<T, Template, First, std::void_t<decltype(detial_::take_template_with<First, Template>(std::declval<T>()))>>
: std::true_type
{};

// test
static_assert(IsTemplateWith<detial_::Test1<int, bool>, detial_::Test1, int>::value);
static_assert(!IsTemplateWith<detial_::Test1<int, bool>, detial_::Test2, int>::value);

template <typename T, typename W>
inline constexpr bool IS_MAP_WITH = Com::IsTemplateWith<T, std::map, W>::value
|| Com::IsTemplateWith<T, std::multimap, W>::value
|| Com::IsTemplateWith<T, std::unordered_map, W>::value
|| Com::IsTemplateWith<T, std::unordered_multimap, W>::value;

template <typename T, typename W>
inline constexpr bool IS_SET_WITH = Com::IsTemplateWith<T, std::set, W>::value
|| Com::IsTemplateWith<T, std::multiset, W>::value
|| Com::IsTemplateWith<T, std::unordered_set, W>::value
|| Com::IsTemplateWith<T, std::unordered_multiset, W>::value;

template <typename Map, typename = void>
struct IsArithmeticKeyedMap
: std::false_type
{};

template <typename Map>
struct IsArithmeticKeyedMap<Map, std::void_t<decltype(std::declval<typename Map::key_type>())>>
: std::integral_constant<bool, IS_MAP<Map> && std::is_arithmetic_v<typename Map::key_type>>
{};

static_assert(IsArithmeticKeyedMap<std::map<int, int>>::value);
static_assert(!IsArithmeticKeyedMap<std::map<std::string, int>>::value);
static_assert(!IsArithmeticKeyedMap<std::set<std::string>>::value);
static_assert(!IsArithmeticKeyedMap<int>::value);

/// test `T` can be used in range-for loop
template <typename T, typename = void>
struct Iterable
: std::false_type
{};

template <typename T>
struct Iterable<T, std::enable_if_t<std::is_array_v<T>>>
: std::true_type
{};

template <typename T>
struct Iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>
: std::true_type
{};

// test
static_assert(Iterable<int[1]>::value);
static_assert(!Iterable<detial_::Test1<int>>::value);

// test T is any type of ...
template <typename T, typename... Any>
struct IsAny
: std::disjunction<std::is_same<T, Any>...>
{};

static_assert(IsAny<int, bool, short, int, double>::value);
static_assert(!IsAny<int, bool, short, double>::value);

/// test `T` has `==` operator
template <typename T, typename = void>
struct HasEqualOp
: std::false_type
{};

template <typename T>
struct HasEqualOp<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
: std::true_type
{};

static_assert(HasEqualOp<int>::value);

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

// hide details
namespace detial_ {

template <typename T, T t>
constexpr char const *get_raw_enum_name()
{
	// NOTE gcc feature only
	return __PRETTY_FUNCTION__;
}

template <typename T, T t>
constexpr std::string_view scrape_enum_name()
{
	std::string_view raw_name = get_raw_enum_name<T, t>();
	auto e = raw_name.find_last_of(']');
	auto b = raw_name.find_last_of(':', e);
	return raw_name.substr(b + 1, e - b - 1);
}

}

template <typename T, T... enums>
std::map<T, std::string_view> const &enum_to_names()
{
	static const std::map<T, std::string_view> map = {
		{enums, detial_::scrape_enum_name<T, enums>()}...
	};
	return map;
}

template <typename T, T... enums>
std::map<std::string_view, T> const &name_to_enum()
{
	static const std::map<std::string_view, T> map = {
		{detial_::scrape_enum_name<T, enums>(), enums}...
	};
	return map;
}
}
#endif
