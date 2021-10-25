#include <poly/Core/Logger.h>
#include <poly/Core/Scheduler.h>

#include <iostream>

namespace poly
{


///////////////////////////////////////////////////////////
Scheduler Scheduler::s_instance;


///////////////////////////////////////////////////////////
Scheduler::Scheduler() :
	m_numBusy		(0),
	m_numStopped	(0),
	m_shouldStop	(false)
{
	Uint32 size = std::thread::hardware_concurrency();
	m_numBusy = size;

	for (Uint32 i = 0; i < size; ++i)
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this, i));

	// Don't continue until all threads are ready
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_numBusy > 0)
			m_fcv.wait(lock);
	}
}


///////////////////////////////////////////////////////////
Scheduler::Scheduler(Uint32 numWorkers) :
	m_numBusy		(0),
	m_shouldStop	(false)
{
	for (Uint32 i = 0; i < numWorkers; ++i)
		m_threads.push_back(std::thread(&Scheduler::workerLoop, this, i));
}


///////////////////////////////////////////////////////////
Scheduler::~Scheduler()
{
	// Automatically stop on destructor
	stop();
}


///////////////////////////////////////////////////////////
void Scheduler::workerLoop(Uint32 id)
{
	Logger::setThreadName("Worker #" + std::to_string(id + 1));

	while (!m_shouldStop)
	{
		priv::TaskStateBase* state = 0;

		{
			// Acquire the mutex to access queue
			std::unique_lock<std::mutex> lock(m_mutex);

			// If there are items in the queue, skip waiting
			if (!m_queue[0].size() && !m_queue[1].size() && !m_queue[2].size())
			{
				// Mark this thread as free
				--m_numBusy;
				m_fcv.notify_all();

				// Wait until get a signal to start work
				m_scv.wait(lock);

				// Mark this thread as busy
				++m_numBusy;

				// Sometimes, threads will wake up without there actually being work
				if (!m_queue[0].size() && !m_queue[1].size() && !m_queue[2].size())
					continue;
			}

			// Get the next function, based on priority, by moving and popping
			// Use std::move because its faster
			if (m_queue[0].size())
			{
				state = m_queue[0].front();
				m_queue[0].pop();
			}
			else if (m_queue[1].size())
			{
				state = m_queue[1].front();
				m_queue[1].pop();
			}
			else
			{
				state = m_queue[2].front();
				m_queue[2].pop();
			}
		}

		// Run the function
		(*state)();
	}

	// Once done, increment the stopped counter
	++m_numStopped;
}


///////////////////////////////////////////////////////////
void Scheduler::finish()
{
	std::unique_lock<std::mutex> lock(s_instance.m_mutex);

	// Keep waiting until number of busy threads is 0 and the size of queue is 0
	while (s_instance.m_numBusy || s_instance.m_queue[0].size() || s_instance.m_queue[1].size() || s_instance.m_queue[2].size())
		s_instance.m_fcv.wait(lock);
}


///////////////////////////////////////////////////////////
void Scheduler::stop()
{
	{
		// Acquire mutex and clear queue to prevent any extra tasks executing
		std::unique_lock<std::mutex> lock(s_instance.m_mutex);

		for (int i = 0; i < 3; ++i)
		{
			while (!s_instance.m_queue[i].empty())
				s_instance.m_queue[i].pop();
		}

		// Wait until all threads are waiting
		while (s_instance.m_numBusy)
			s_instance.m_fcv.wait(lock);
	}

	// Set stop flag
	s_instance.m_shouldStop = true;

	do
		// Sometimes (very rarely), the stopping thread will notify the conditional variable
		// before all the worker threads get to their wait.
		// It will cause the program to freeze because without sending the signal more than
		// once, the thread will wait until a signal from another source is recieve

		// Loop the notify until all threads are stopped
		s_instance.m_scv.notify_all();
	while (s_instance.m_numStopped < s_instance.m_threads.size());

	// Join all threads
	for (Uint32 i = 0; i < s_instance.m_threads.size(); ++i)
	{
		if (s_instance.m_threads[i].joinable())
			s_instance.m_threads[i].join();
	}
}


}