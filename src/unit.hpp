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
#ifndef COM_SRC_UNIT_HPP_
#define COM_SRC_UNIT_HPP_
#include <cassert>

/// Basic unit testing utils

#define MAIN int main(int argc, const char *argv[]) { return 0; }

/// do testing in following block:
///
/// ```C++
/// TEST(do_somthing, "test function do_something")
/// {
/// 	// your testing code
/// }
/// ```
#define TEST(name, ...) struct name##_test_type {\
	name##_test_type(); \
}; \
static name##_test_type name##_test_obj; \
inline name##_test_type::name##_test_type()

#endif
