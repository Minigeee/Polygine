#ifndef POLY_SHCEDULER_H
#define POLY_SHCEDULER_H

#include <poly/Core/DataTypes.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace poly
{

class Scheduler
{
public:
	Scheduler();
	Scheduler(Uint32 numWorkers);
	~Scheduler();

	template <typename F, typename... Args>
	void addTask(F&& func, Args&&... args);

	void finish();
	void stop();

private:
	void workerLoop();

	std::queue<std::function<void()>> m_queue;
	std::vector<std::thread> m_threads;
	std::atomic<Uint32> m_numBusy;
	std::atomic<bool> m_shouldStop;

	std::mutex m_mutex;
	std::condition_variable m_scv;
	std::condition_variable m_fcv;
};

#include <poly/Core/Scheduler.inl>

}

#endif