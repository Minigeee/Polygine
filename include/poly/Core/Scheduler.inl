namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline Task<T>::Task(TaskImpl* impl):
	m_impl		(impl)
{
	m_impl->m_refCount = 2;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Task<T>::Task(Task<T>&& other) :
	m_impl		(other.m_impl)
{
	other.m_impl = 0;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Task<T>& Task<T>::operator=(Task<T>&& other)
{
	if (&other != this)
	{
		if (m_impl && --m_impl->m_refCount == 0)
			delete m_impl;

		m_impl = other.m_impl;
		other.m_impl = 0;
	}

	return *this;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Task<T>::~Task()
{
	if (--m_impl->m_refCount == 0)
		delete m_impl;
	m_impl = 0;
}


///////////////////////////////////////////////////////////
template <typename T>
inline T& Task<T>::getResult() const
{
	return m_impl->m_result;
}


///////////////////////////////////////////////////////////
template <typename T>
inline bool Task<T>::isFinished() const
{
	return m_impl->m_refCount == 1;
}


///////////////////////////////////////////////////////////
inline Task<void>::Task(TaskImpl* impl) :
	m_impl(impl)
{
	m_impl->m_refCount = 2;
}


///////////////////////////////////////////////////////////
inline Task<void>::~Task()
{
	if (--m_impl->m_refCount == 0)
		delete m_impl;
	m_impl = 0;
}


///////////////////////////////////////////////////////////
inline Task<void>::Task(Task<void>&& other) :
	m_impl		(other.m_impl)
{
	other.m_impl = 0;
}


///////////////////////////////////////////////////////////
inline Task<void>& Task<void>::operator=(Task<void>&& other)
{
	if (&other != this)
	{
		if (m_impl && --m_impl->m_refCount == 0)
			delete m_impl;

		m_impl = other.m_impl;
		other.m_impl = 0;
	}

	return *this;
}


///////////////////////////////////////////////////////////
inline bool Task<void>::isFinished() const
{
	return m_impl->m_refCount == 1;
}


///////////////////////////////////////////////////////////
template <typename F, typename... Args, typename Ret>
inline Task<Ret> Scheduler::addTask(F&& func, Args&&... args)
{
	return addTask(High, std::forward<F>(func), std::forward<Args>(args)...);
}


///////////////////////////////////////////////////////////
template <typename F, typename... Args, typename Ret>
inline typename std::enable_if<std::is_same<Ret, void>::value, Task<Ret>>::type Scheduler::addTask(Scheduler::Priority priority, F&& func, Args&&... args)
{
	// Task
	Task<Ret>::TaskImpl* impl = new Task<Ret>::TaskImpl();

	auto f = [&, impl]()
	{
		func(args...);

		if (--impl->m_refCount == 0)
			delete impl;
	};

	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue[priority].push(std::move(f));
	}

	// Notify any threads that are ready
	m_scv.notify_one();

	return Task<Ret>(impl);
}


///////////////////////////////////////////////////////////
template <typename F, typename... Args, typename Ret>
inline typename std::enable_if<!std::is_same<Ret, void>::value, Task<Ret>>::type Scheduler::addTask(Scheduler::Priority priority, F&& func, Args&&... args)
{
	// Task
	Task<Ret>::TaskImpl* impl = new Task<Ret>::TaskImpl();

	auto f = [&, impl]()
	{
		impl->m_result = func(args...);

		if (--impl->m_refCount == 0)
			delete impl;
	};

	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue[priority].push(std::move(f));
	}

	// Notify any threads that are ready
	m_scv.notify_one();

	return Task<Ret>(impl);
}


}