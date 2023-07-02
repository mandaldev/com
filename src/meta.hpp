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
#ifndef _COM_SRC_META_HPP_
#define _COM_SRC_META_HPP_
#include <cstdint>
#include <type_traits>
#include <iterator>

namespace Com {

// hide details
namespace __detial {

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
struct IsTemplate<T, Template, std::void_t<decltype(__detial::take_template<Template>(std::declval<T>()))>>
: std::true_type
{};

// test
static_assert(IsTemplate<__detial::Test1<int>, __detial::Test1>::value);
static_assert(!IsTemplate<__detial::Test1<int>, __detial::Test2>::value);

/// test `T` is a template type `Template<First, ...>`
template <typename T, template <typename...> typename Template, typename First, typename = void>
struct IsTemplateWith
: std::false_type
{};

template <typename T, template <typename...> typename Template, typename First>
struct IsTemplateWith<T, Template, First, std::void_t<decltype(__detial::take_template_with<First, Template>(std::declval<T>()))>>
: std::true_type
{};

// test
static_assert(IsTemplateWith<__detial::Test1<int, bool>, __detial::Test1, int>::value);
static_assert(!IsTemplateWith<__detial::Test1<int, bool>, __detial::Test2, int>::value);

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
static_assert(!Iterable<__detial::Test1<int>>::value);

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

}
#endif
