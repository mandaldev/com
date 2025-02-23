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
#ifndef MAS_SRC_VALUE_HPP
#define MAS_SRC_VALUE_HPP
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

/// A simple Mas text.
///
/// ```
/// {
///   "string as key" [ "this" "is" "ok" ]
///   key1 [ "special" "key" "can" "avoid" "quoting" ]
///   map_value {
///     1 "a nested map"
///   }
/// }
/// ```
///
/// As that: Mas stands for Map(M), Array(A), and String(S).
/// So simple that no ',' marks needed.
///
/// value: map | array | string
/// map: '{' ws key_value_pairs ws '}'
/// array: '[' ws values ws ']'
/// string: '"' escaped_bytes '"'
/// key_value_pairs: key ws value
/// key: string | key_chars
/// key_chars: key_char+
/// values: value+
/// escaped_bytes: (escaped_char | ~(quote | escape))*
/// quote: '"'
/// escape: '\'
/// escaped_char:
/// 	": '\"'
/// 	\: '\\'
/// 	bell (0x07): '\a'
/// 	backspace (0x08): '\b'
/// 	new page (0x0c): '\f'
/// 	lf (0x0a): '\n'
/// 	cr (0x0d): '\r'
/// 	tab (0x09): '\t'
/// 	vtab (0x0b): '\v',
/// 	others will be retained
///
/// ws: white_space*
/// key_char: ascii chars range : from '!' to '~' (33 ~ 126),  except: "{", "}", "[", "]", and quote '"'

namespace Com::Mas {

class Value;

/// Mas string type, same as std::string
using String = std::string;
/// Mas array type, vector of Value
using Array = std::vector<Value>;
/// Mas map type, map of String-Value pair
using Map = std::map<String, Value>;

/// encode option struct.
/// default is best for performence
struct EncodeOption
{
	/// use pretty output (add new lines and indentations)
	bool pretty = false;
	/// force key string quoting
	bool force_quoting = true;
}; // struct EncodeOption

/// Encode a Mas value to text
std::string encode(Value const &value, EncodeOption const &option = {});

/// Decode a Mas value from text
std::optional<Value> decode(std::string const &mas_text);

/// Wrapper class of MAS union
class Value
{
	friend class Decoder;

	/// union of MAS
	std::variant<Map, Array, String> _var;

public:
	Value() = default;
	Value(Map const &src): _var(src) {}
	Value(Array const &src): _var(src) {}
	Value(String const &src): _var(src) {}
	Value(char const *src): _var(String(src)) {}

	/// type checker

	bool is_map() const;
	bool is_array() const;
	bool is_string() const;

	/// value getter, assign with `src` if type does't match

	Map &as_map(Map const &src = {});
	Array &as_array(Array const &src = {});
	String &as_string(String const &src = {});

	/// const value getter, return `what` if type does't match

	Map const &map_or(Map const &what) const;
	Array const &array_or(Array const &what) const;
	String const &string_or(String const &what) const;

	template <typename Visitor>
	auto visit_by(Visitor &visitor) const
	{
		return std::visit(visitor, _var);
	}
};

bool get_string(Mas::Map const &m, std::string const &key, std::string &value);

} // namespace Mas

#endif
