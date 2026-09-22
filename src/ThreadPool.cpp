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
#include "log.hpp"
#include <cstdint>
#include <exception>
#include <mutex>

namespace Com {

ThreadPool::ThreadPool(unsigned threads)
{
	_max_threads = std::max(_max_threads, threads);
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

	if (_thrds.size() == 0 || (_busy_thrd_count == _thrds.size() && _thrds.size() < _max_threads)) {
		_thrds.emplace_back([this]()
		{
			worker();
		});
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
	{
		std::unique_lock<std::mutex> locker(_mtx);
		_tasks.clear();
	}

	_runing = false;
	_cv.notify_all();
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
				return !_runing || _tasks.size() > 0;
			});
			if (_runing && _tasks.size()) {
				task = _tasks.front().second;
				_tasks.pop_front();
			}
		}
		if (task) {
			_busy_thrd_count += 1;
			try {
				task();
			} catch (std::exception &e) {
				LOG_ERROR << "task throwed an exception: " << e.what();
			}
			_busy_thrd_count -= 1;
		}
	}
}

}
