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
#ifndef COM_SRC_PROC_MOUNTS_HPP
#define COM_SRC_PROC_MOUNTS_HPP
#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace Com::Proc {

/// `/proc/mounts`: https://www.man7.org/linux/man-pages/man5/proc_mounts.5.html
class Mounts
{
public:
	struct Mount {
		/// 挂载点
		std::filesystem::path location;
		/// 设备
		std::string device;
		/// 文件系统类型
		std::string fs_type;
		/// 挂载选项
		std::vector<std::string> opts;
		std::string freq;
		std::string passno;
	};

	/// 从 `/proc/mounts` 读取最新数据
	bool load();

	std::vector<Mount> const &get_data() const;

	/// 找到 `path` 所在的文件系统的挂载点，返回 mount 条目的索引，
	/// 未找到时返回的索引为非法值。
	size_t find_mount_location(std::filesystem::path const &path) const;

private:
	std::vector<Mount> _data;
	std::map<std::filesystem::path, size_t> _map;
};

}
#endif
