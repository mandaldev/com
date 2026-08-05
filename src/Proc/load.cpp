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
#include "load.hpp"
#include "../file.hpp"
#include "../string.hpp"
#include "../Range.hpp"
#include <string>

namespace Com::Proc {

std::vector<std::pair<std::string, std::string>>
load_header_value_style(std::filesystem::path const &path, std::string const &sep)
{
	auto opt = load_file(path);
	if (!opt) {
		return {};
	}
	std::vector<std::pair<std::string, std::string>> ret;
	auto lines = split(std::string_view(opt.value()), "\n");
	for (auto line : lines) {
		line = trim(line);
		if (line.empty()) {
			continue;
		}
		auto fields = split(line, sep);
		if (fields.size() < 2) {
			continue;
		}
		auto &data = ret.emplace_back();
		data.first = trim(fields[0]);
		if (fields.size() > 2) {
			data.second = trim(join(":", make_range(fields.begin() + 1, fields.end())));
		} else {
			data.second = trim(fields[1]);
		}

	}
	return ret;
}

std::vector<std::vector<std::string>>
load_header_values_style(std::filesystem::path const &path, std::string const &sep)
{
	auto opt = load_file(path);
	if (!opt) {
		return {};
	}
	std::vector<std::vector<std::string>> ret;
	auto lines = split(std::string_view(opt.value()), "\n");
	for (auto line : lines) {
		line = trim(line);
		if (line.empty()) {
			continue;
		}
		auto fields = split(line, sep);
		auto &data = ret.emplace_back();
		for (auto field : fields) {
			data.push_back(std::string(trim(field)));
		}

	}
	return ret;
}

std::vector<std::vector<std::string>>
load_values_style(std::filesystem::path const &path, std::string const &sep)
{
	auto opt = load_file(path);
	if (!opt) {
		return {};
	}
	std::vector<std::vector<std::string>> ret;
	auto lines = split(std::string_view(opt.value()), "\n");
	for (auto line : lines) {
		auto fields = split(line, sep);
		auto &data = ret.emplace_back();
		for (auto field : fields) {
			data.push_back(std::string(trim(field)));
		}

	}
	return ret;
}

}
