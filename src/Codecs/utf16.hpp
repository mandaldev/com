// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2026  Mandal Dev
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
#ifndef COM_SRC_CODECS_UTF16_HPP
#define COM_SRC_CODECS_UTF16_HPP
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

/// UTF-16 codecs.
/// NOTE no support for BOM

namespace Com {

std::optional<std::u16string> utf16_encode(std::u32string_view str);

std::optional<std::u32string> utf16_decode(void const *buf, size_t len, bool little_endian = true);

std::optional<std::u32string> utf16_decode(std::u16string_view str);

}
#endif
