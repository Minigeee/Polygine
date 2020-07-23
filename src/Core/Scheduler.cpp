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
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this));
}

Scheduler::Scheduler(Uint32 numWorkers) :
	m_numBusy		(0),
	m_shouldStop	(false)
{
	for (Uint32 i = 0; i < numWorkers; ++i)
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this));
}

Scheduler::~Scheduler()
{
	// Automatically stop on destructor
	stop();
}

///////////////////////////////////////////////////////////

void Scheduler::workerLoop()
{
	while (!m_shouldStop)
	{
		std::function<void()> fn;

		{
			// First, aquire lock
			std::unique_lock<std::mutex> lock(m_mutex);

			// Then check if there are tasks in queue
			if (!m_queue.size())
			{
				// If there aren't any tasks, wait for one
				m_scv.wait(lock);

				// Check if queue is still empty
				if (!m_queue.size())
					// If it is empty, then reset
					continue;
			}

			// Now get the next task and pop
			fn = std::move(m_queue.front());
			m_queue.pop();
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
	while (m_numBusy || m_queue.size())
		m_fcv.wait(lock);
}

void Scheduler::stop()
{
	m_shouldStop = true;
	m_scv.notify_all();

	for (Uint32 i = 0; i < m_threads.size(); ++i)
	{
		if (m_threads[i].joinable())
			m_threads[i].join();
	}
}

///////////////////////////////////////////////////////////

}