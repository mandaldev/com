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
#include "ThreadPool.hpp"
#include <cstdint>
#include <mutex>

namespace Com {

ThreadPool::ThreadPool(uint16_t threads)
{
	_max_threads = std::max(_max_threads, threads);
	_thrds.reserve(_max_threads);
	_runing = true;

	for (uint16_t i = 0; i < _max_threads; i++) {
		_thrds.emplace_back(bind_weak([this]()
		{
			worker();
		}));
	}
}

ThreadPool::~ThreadPool()
{
	stop(true);
}

uint64_t ThreadPool::add_task(Task const &task)
{
	uint64_t id = 0;
	{
		std::lock_guard<std::mutex> locker(_mtx);
		id = ++_new_task_id;
		_tasks.push_back({id, task});
	}

	_cv.notify_one();
	return id;
}

bool ThreadPool::cancel_task(uint64_t task_id)
{
	std::lock_guard<std::mutex> locker(_mtx);
	for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (it->first == task_id) {
			_tasks.erase(it);
			return true;
		}
	}
	return false;
}

void ThreadPool::stop(bool wait)
{
	_runing = false;
	if (wait) {
		for (auto &t : _thrds) {
			if (t.joinable()) {
				t.join();
			}
		}
	}
}

void ThreadPool::worker()
{
	while (_runing) {
		Task task;
		{
			std::unique_lock<std::mutex> locker(_mtx);
			_cv.wait(locker, [this]()
			{
				return _tasks.size() > 0;
			});
			if (_tasks.size()) {
				task = _tasks.front().second;
				_tasks.pop_front();
			}
		}
		if (task) {
			task();
		}
	}
}

}
