// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2024  Mandal Dev
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
#ifndef COM_SRC_LOG_HPP_
#define COM_SRC_LOG_HPP_
#include <chrono>
#include <iomanip>
#include <iostream>

/// a simple logger, to std::cerr

// hide log impl detial

namespace Com::v_logger_ {

struct FakeLogger
{
	FakeLogger(char const *, int, const char *, const char *) {}
	template <typename T> FakeLogger &operator<<(T const &) { return *this; }
};

struct Logger
{
	Logger(char const *file, int line, const char *func, const char *severity)
	{
		auto t_now = std::chrono::system_clock::now();
		auto us = std::chrono::duration_cast<std::chrono::microseconds>(t_now.time_since_epoch()).count();
		auto now = std::chrono::system_clock::to_time_t(t_now);
		auto t_str = std::put_time(std::localtime(&now), "%F %T");
		std::cerr
			<< "[" << severity << "]"
			<< "[" << t_str << '.' << (us % 1000000) << "]"
			<< "[" << file << ":" << line << ":" << func << "] "
			;
	}

	~Logger()
	{
		std::cerr << "\n";
	}

	template <typename T>
	Logger &operator<<(T const &t)
	{
		std::cerr << t;
		return *this;
	}
};

#ifndef NDEBUG
#define LOG_DEBUG Com::v_logger_::Logger(__FILE__, __LINE__, __func__, "Debug")
#else
#define LOG_DEBUG Com::v_logger_::FakeLogger(__FILE__, __LINE__, __func__, "Debug")
#endif
#define LOG_INFO Com::v_logger_::Logger(__FILE__, __LINE__, __func__, "Info")
#define LOG_WARN Com::v_logger_::Logger(__FILE__, __LINE__, __func__, "Warn")
#define LOG_ERROR Com::v_logger_::Logger(__FILE__, __LINE__, __func__, "Error")
#define LOG_FATAL Com::v_logger_::Logger(__FILE__, __LINE__, __func__, "Fatal")

}
#endif
