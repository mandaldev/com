// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2025  Mandal Dev
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
#ifndef COM_SRC_THREADPOOL_HPP
#define COM_SRC_THREADPOOL_HPP
#include "raii.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

namespace Com {

class ThreadPool
: Com::Unique
{
public:
	using Task = std::function<void()>;

	/// 默认 1 个线程
	ThreadPool(unsigned threads);

	~ThreadPool();

	/// add task to pool, return task id
	uint64_t add_task(Task const &task);

	/// cancel task, return true if task is removed from pending list
	bool cancel_task(uint64_t task_id);

	/// must call this to stop runing
	void stop(bool wait);

private:
	void worker();

	unsigned _max_threads = 1;
	std::atomic_bool _runing{true};
	std::atomic_uint _busy_thrd_count{0};
	std::vector<std::thread> _thrds;

	std::mutex _mtx;
	std::condition_variable _cv;
	uint64_t _new_task_id = 0;
	std::list<std::pair<uint64_t, Task>> _tasks;
};

}
#endif
