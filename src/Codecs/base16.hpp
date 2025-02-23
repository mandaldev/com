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
#ifndef DEV_SRC_V_CODECS_BASE16_HPP_
#define DEV_SRC_V_CODECS_BASE16_HPP_
#include <optional>
#include <string>

namespace Com {

/// base16 codecs
/// encode byte to hex form XX.
/// decode "XX" string to byte.
///
/// to be easy to use, these api use raw pointers, they must not be nullptrs
/// cause codecs never check them.

std::string base16_encode(void const *buf, size_t size);

/// WARNING: outbuf size must be not less then `2 * in_size`
void base16_encode(void const *in_buf, size_t in_size, char *out_buf);

std::optional<std::string> base16_decode(std::string const &buf);

/// WARNING: outbuf size must be not less then `in_size / 2`
bool base16_decode(char const *in_buf, size_t in_size, void *out_buf);

}
#endif
