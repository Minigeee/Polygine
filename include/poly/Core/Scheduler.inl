namespace poly
{

///////////////////////////////////////////////////////////

template <typename F, typename... Args>
void Scheduler::addTask(F&& func, Args&&... args)
{
	// Create the function
	auto f = std::bind(std::forward<F>(func), std::forward<Args>(args)...);

	// Add to queue
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue[0].push(std::move(f));
	}

	// Notify any threads that are ready
	m_scv.notify_one();
}

template <typename F, typename... Args>
void Scheduler::addTask(Scheduler::Priority priority, F&& func, Args&&... args)
{
	// Create the function
	auto f = std::bind(std::forward<F>(func), std::forward<Args>(args)...);

	// Add to queue
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue[priority].push(std::move(f));
	}

	// Notify any threads that are ready
	m_scv.notify_one();
}

///////////////////////////////////////////////////////////

}