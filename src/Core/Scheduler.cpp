#include <poly/Core/Logger.h>
#include <poly/Core/Scheduler.h>

namespace poly
{

///////////////////////////////////////////////////////////

Scheduler::Scheduler() :
	m_numBusy		(0),
	m_shouldStop	(false)
{
	Uint32 size = std::thread::hardware_concurrency();
	for (Uint32 i = 0; i < size; ++i)
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this, i));
}

Scheduler::Scheduler(Uint32 numWorkers) :
	m_numBusy		(0),
	m_shouldStop	(false)
{
	for (Uint32 i = 0; i < numWorkers; ++i)
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this, i));
}

Scheduler::~Scheduler()
{
	// Automatically stop on destructor
	stop();
}

///////////////////////////////////////////////////////////

void Scheduler::workerLoop(Uint32 id)
{
	Logger::setThreadName("Worker #" + std::to_string(id));

	while (!m_shouldStop)
	{
		std::function<void()> fn;

		{
			// First, aquire lock
			std::unique_lock<std::mutex> lock(m_mutex);

			// Then check if there are tasks any of the queues
			if (!m_queue[0].size() && !m_queue[1].size() && !m_queue[2].size())
			{
				// If there aren't any tasks, wait for one
				m_scv.wait(lock);

				// Check if the queues are still empty
				if (!m_queue[0].size() && !m_queue[1].size() && !m_queue[2].size())
					// If all are empty, then reset
					continue;
			}

			// Now get the next task, starting with highest priority queue, and pop
			if (m_queue[0].size())
			{
				fn = std::move(m_queue[0].front());
				m_queue[0].pop();
			}
			else if (m_queue[1].size())
			{
				fn = std::move(m_queue[1].front());
				m_queue[1].pop();
			}
			else
			{
				fn = std::move(m_queue[2].front());
				m_queue[2].pop();
			}
		}

		// Indicate that a thread is busy
		++m_numBusy;

		// Start the task
		fn();

		// After finishing, indicate that a thread is available
		--m_numBusy;
		m_fcv.notify_all();
	}
}

///////////////////////////////////////////////////////////

void Scheduler::finish()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	// Keep waiting until number of busy threads is 0 and the size of queue is 0
	while (m_numBusy || m_queue[0].size() || m_queue[1].size() || m_queue[2].size())
		m_fcv.wait(lock);
}

void Scheduler::stop()
{
	{
		// Acquire mutex and clear queue to prevent any extra tasks executing
		std::unique_lock<std::mutex> lock(m_mutex);

		for (int i = 0; i < 3; ++i)
		{
			while (!m_queue[i].empty())
				m_queue[i].pop();
		}
	}

	// Notify stop
	m_shouldStop = true;
	m_scv.notify_all();

	// Join all threads
	for (Uint32 i = 0; i < m_threads.size(); ++i)
	{
		if (m_threads[i].joinable())
			m_threads[i].join();
	}
}

///////////////////////////////////////////////////////////

}