#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <queue>
#include <vector>

// https://www.youtube.com/watch?v=eWTGtp3HXiw

template <class T>
class Task
{
public:
	Task()
	{
	}

	Task(std::function<void(std::shared_ptr<T>)> func, std::shared_ptr<T> data)
		: Func{ func }, Data{ data }
	{
	}

	std::function<void(std::shared_ptr<T>)> Func;
	std::shared_ptr<T> Data;

	void execute()
	{
		//std::cout << "\nTask #" << Data->chunkId << " starting...";
		//std::this_thread::sleep_for(250ms);
		Func(Data);
		//std::cout << "\nTask #" << Data->chunkId << " finished.";
	}

};

template <class T>
class ThreadPool
{
public:
	explicit ThreadPool(std::size_t numThreads)
	{
		start(numThreads);
	}

	~ThreadPool()
	{
		stop();
	}

	void enqueue(Task<T> task)
	{
		{
			std::unique_lock<std::mutex>{_eventMutex};
			_tasks.emplace(std::move(task));
		}

		_eventVar.notify_one();

	}

	bool hasJobs()
	{
		return !_tasks.empty();
	}

	std::size_t numJobsInQueue()
	{
		return _tasks.size();
	}

private:
	std::vector<std::thread> _threads;

	std::condition_variable _eventVar;
	std::mutex _eventMutex;
	bool _stopping{ false };

	std::queue<Task<T>> _tasks;

	void start(std::size_t numThreads)
	{
		for (auto i = 0u; i < numThreads; ++i)
		{
			_threads.emplace_back([=] {
				while (true)
				{
					Task<T> task;

					{
						std::unique_lock<std::mutex> lock{ _eventMutex };
						_eventVar.wait(lock, [=] {return _stopping || !_tasks.empty(); });

						if (_stopping && _tasks.empty())
							break;

						task = std::move(_tasks.front());
						_tasks.pop();
					}

					task.execute();
				}
				});
		}
	}

	void stop() noexcept
	{
		{
			std::unique_lock<std::mutex> lock{ _eventMutex };
			_stopping = true;
		}

		_eventVar.notify_all();

		for (auto& thread : _threads)
			thread.join();
	}
};

#endif