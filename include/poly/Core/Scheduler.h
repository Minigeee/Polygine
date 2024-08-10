#ifndef POLY_SHCEDULER_H
#define POLY_SHCEDULER_H

#include <poly/Core/DataTypes.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

namespace poly
{

template <typename Ret> class TaskBase;
class Scheduler;

#ifndef DOXYGEN_SKIP
namespace priv
{


///////////////////////////////////////////////////////////
/// \brief The base class for task states
///
///////////////////////////////////////////////////////////
class TaskStateBase
{
	template <typename Ret>
	friend class TaskBase;

public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~TaskStateBase() { }

	///////////////////////////////////////////////////////////
	/// \brief Virtual function operator
	///
	///////////////////////////////////////////////////////////
	virtual void operator()() = 0;

protected:
	std::atomic_int m_refCount;		//!< A reference counter to help with lifetime management
};


///////////////////////////////////////////////////////////
/// \brief A task state base type with result type
///
///////////////////////////////////////////////////////////
template <typename Ret>
class TaskStateResultType : public TaskStateBase
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~TaskStateResultType() { }

	///////////////////////////////////////////////////////////
	/// \brief Get the result from the task state
	///
	///////////////////////////////////////////////////////////
	Ret& getResult();

protected:
	Ret m_result;		//!< The funtion return value
};

template <> class TaskStateResultType<void> : public TaskStateBase { };


}
#endif


///////////////////////////////////////////////////////////
/// \brief The base class for scheduler tasks
///
///////////////////////////////////////////////////////////
template <typename Ret>
class TaskBase
{
	friend Scheduler;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates a task with no associated task state
	///
	///////////////////////////////////////////////////////////
	TaskBase();

#ifndef DOXYGEN_SKIP
	TaskBase(const TaskBase&) = delete;
	TaskBase& operator=(const TaskBase&) = delete;
	TaskBase(TaskBase&& other);
	TaskBase& operator=(TaskBase&& other);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~TaskBase();

	///////////////////////////////////////////////////////////
	/// \brief Check if associated scheduler task has finished executing
	///
	/// \return True if the scheduler task has finished executing
	///
	///////////////////////////////////////////////////////////
	bool isFinished() const;

protected:
	priv::TaskStateResultType<Ret>* m_state;	//!< The associated task state
};


///////////////////////////////////////////////////////////
/// \brief A class used to check status and get results of a scheduler task
///
///////////////////////////////////////////////////////////
template <typename Ret>
class Task : public TaskBase<Ret>
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Get the return value of a scheduler task
	///
	/// The returned reference will contain the value returned from
	/// the schdduler task if the task has finished executing, or the
	/// default value if it has not. The running status of a task can
	/// be checked with isFinished().
	///
	/// This function does not exist when the return type is void.
	///
	/// \return A reference to the task return value
	///
	///////////////////////////////////////////////////////////
	Ret& getResult();
};

template <> class Task<void> : public TaskBase<void> { };


///////////////////////////////////////////////////////////
/// \brief A class that distributes tasks to several worker threads
///
///////////////////////////////////////////////////////////
class Scheduler
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Priority levels for the tasks
	///
	///////////////////////////////////////////////////////////
	enum Priority
	{
		High,	//!< High priority tasks will be executed first
		Medium, //!< Medium priority tasks will be executed before low priority
		Low		//!< Low priority tasks will be executed last
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// The default constructor creates the system recommendend
	/// amount of worker threads, using std::thread::hardware_concurrency().
	/// Note that after initial construction, the number of
	/// worker threads cannot be changed. To specify the number
	/// of worker threads, use the other constructor.
	///
	///////////////////////////////////////////////////////////
	Scheduler();

	///////////////////////////////////////////////////////////
	/// \brief Construct scheduler with a certain number of worker threads
	///
	/// Note that after initial construction, the number of
	/// worker threads cannot be changed.
	///
	///////////////////////////////////////////////////////////
	Scheduler(Uint32 numWorkers);

	///////////////////////////////////////////////////////////
	/// \brief Destructor makes sure that all worker threads have finished
	///
	/// The destructor will block the current thread until
	/// all worker threads have finished their current tasks,
	/// and have joined into the calling thread.
	///
	/// \see stop
	///
	///////////////////////////////////////////////////////////
	~Scheduler();

#ifndef DOXYGEN_SKIP
	Scheduler(const Scheduler&)				= delete;
	Scheduler& operator=(const Scheduler&)	= delete;
	Scheduler(Scheduler&&)					= default;
	Scheduler& operator=(Scheduler&&)		= default;
#endif

	///////////////////////////////////////////////////////////
	/// \brief Add a task function for the scheduler to execute
	///
	/// This function adds the specified function to a queue
	/// for the worker threads to execute when available.
	/// The return value of the function and the running status
	/// of the function can be checked through returned Task
	/// object.
	///
	/// In order to call the task function at a later time, when
	/// the current local variables may have gone out of scope, all arguments
	/// that are an rvalue reference or a const lvalue reference are
	/// copied into an internal tuple. Lvalue references that are not
	/// const are kept as references, so undefined behavior will
	/// occur if the stored references go out of scope.
	///
	/// To add a normal function:
	///
	/// \li addTask(func, arg1, arg2, arg3, ...);
	///
	/// To add a member function:
	///
	/// \li addTask(&Class::func, &instance, arg1, arg2, ...);
	///
	/// For member functions, the instance passed in can be pointer or
	/// reference.
	///
	/// All tasks added using this function will use
	/// a Priority::High. To specify a different priority level,
	/// use the overloaded version of this function.
	///
	/// \param func The function to execute
	/// \param args All other arguments for the specified function
	///
	/// \return A Task obejct that can be used to retrieve the function return value
	///
	///////////////////////////////////////////////////////////
	template <typename F, typename... Args, typename Ret = typename std::result_of<F(Args...)>::type>
	static Task<Ret> addTask(F&& func, Args&&... args);

	///////////////////////////////////////////////////////////
	/// \brief Add a task function with a certain priority for the scheduler to execute
	///
	/// This function adds the specified function to a queue
	/// for the worker threads to execute when available.
	/// The return value of the function and the running status
	/// of the function can be checked through returned Task
	/// object.
	///
	/// In order to call the task function at a later time, when
	/// the current local variables may have gone out of scope, all arguments
	/// that are an rvalue reference or a const lvalue reference are
	/// copied into an internal tuple. Lvalue references that are not
	/// const are kept as references, so undefined behavior will
	/// occur if the stored references go out of scope.
	///
	/// To add a normal function:
	///
	/// \li addTask(func, arg1, arg2, arg3, ...);
	///
	/// To add a member function:
	///
	/// \li addTask(&Class::func, &instance, arg1, arg2, ...);
	///
	/// For member functions, the instance passed in can be pointer or
	/// reference.
	///
	/// Tasks with higher priority will be executed before tasks
	/// with lower priority.
	///
	/// \li \link Priority::High \endlink should be used for tasks that need to be
	/// finished as soon as possible
	/// \li \link Priority::Medium \endlink should be used for tasks that need
	/// to be finished soon, but are not as important.
	/// \li \link Priority::Low \endlink should be used for tasks that can be
	/// finished whenever
	///
	/// \param priority The #Priority level to execute the task with
	/// \param func The function to execute
	/// \param args All other arguments for the specified function
	///
	/// \return A Task obejct that can be used to retrieve the function return value
	///
	///////////////////////////////////////////////////////////
	template <typename F, typename... Args, typename Ret = typename std::result_of<F(Args...)>::type>
	static Task<Ret> addTask(Priority priority, F && func, Args&&... args);

	///////////////////////////////////////////////////////////
	/// \brief Wait for all tasks in the queue to finish
	///
	/// This function will block the calling thread until all
	/// current tasks and tasks in the queue have finished.
	///
	///////////////////////////////////////////////////////////
	static void finish();

	///////////////////////////////////////////////////////////
	/// \brief Clears the task queue and stops all worker threads
	///
	/// This function clears the task queue to prevent any extra
	/// tasks from being executed. Then it blocks the calling
	/// thread until all tasks that are currently being run are
	/// and have joined the current thread.
	///
	///////////////////////////////////////////////////////////
	static void stop();

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the worker thread pool
	///
	/// This will determine how many worker threads will be used
	/// to execute tasks. If a positive number of worker threads already
	/// exist, then the scheduler will stop(), then resize the thread pool.
	///
	/// \param num The number of worker threads that will be used
	///
	///////////////////////////////////////////////////////////
	static void setNumWorkers(Uint32 num);

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the worker thread pool
	///
	/// \return The number of worker threads that have been created
	///
	///////////////////////////////////////////////////////////
	static Uint32 getNumWorkers();

private:
	///////////////////////////////////////////////////////////
	/// \brief The loop that worker threads use
	///
	///////////////////////////////////////////////////////////
	void workerLoop(Uint32 id);

private:
	std::queue<priv::TaskStateBase*> m_queue[3];	//!< The task queue
	std::vector<std::thread> m_threads;				//!< The list of worker threads
	std::atomic<Uint32> m_numBusy;					//!< The number of busy threads
	std::atomic<Uint32> m_numStopped;				//!< The number of threads that have left their loop
	std::atomic<bool> m_shouldStop;					//!< True if stop() has been called

	std::mutex m_mutex;								//!< Mutex to protect queue and for condition variables
	std::condition_variable m_scv;					//!< The condition variable used to notify new tasks (start)
	std::condition_variable m_fcv;					//!< The condition variable used to notify finishing tasks (finish)

	static Scheduler s_instance;					//!< Singleton
};


}

#include <poly/Core/Scheduler.inl>

#endif


///////////////////////////////////////////////////////////
/// \class poly::Task
/// \ingroup Core
///
/// This class provides a way to check if a function executed
/// through Scheduler::addTask() has finished, and it provides
/// a way to retrieve the return value of the function.
///
/// A Task is moveable but not copyable.
///
/// For a usage example, plase check the documentation for Scheduler.
///
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \class poly::Scheduler
/// \ingroup Core
///
/// Scheduler is a class that distributes task functions to
/// worker threads. Tasks are added to a queue, which are then
/// executed by the worker threads when available. There are
/// a limited number of threads that are created in the
/// constructor. These threads are never stopped
/// and no new threads are ever created during the lifetime of
/// the Scheduler, to minimize the overhead of creating
/// and destroying threads.
///
/// The default constructor creates a certain number of threads,
/// based on std::thread::hardware_concurrency(), but the
/// number of threads can be specified in the constructor as
/// well.
///
/// Upon destruction or when stop() is called, the task queue
/// is cleared and the calling thread is blocked until all
/// current tasks have finished and joined.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Use a mutex to protect std::cout
/// std::mutex m;
///
/// class A
/// {
///		void test(const std::string& str)
///		{
///			// Lock the mutex so only 1 thread can use std::cout at a time
///			std::lock_guard<std::mutex> lock(m);
///			std::cout << str << " is being called from a class\n";
///		}
/// }
///
/// void test(const std::string& str)
/// {
///		// Lock the mutex so only 1 thread can use std::cout at a time
///		std::lock_guard<std::mutex> lock(m);
///		std::cout << "Hello " << str << "!\n";
/// }
///
/// float add(float a, float b)
/// {
///		return a + b;
/// }
///
/// int main()
/// {
///		// These tasks will be added into the high priority queue
///		Scheduler::addTask(test, "World");
///		Scheduler::addTask(test, "ABC");
///
///
///		// Add a low priority task
///		Scheduler::addTask(Scheduler::Low, test, "Low");
///		// Even though this task was added after the low priority, it will execute first
///		Scheduler::addTask(Scheduler::Medium, test, "Medium");
///
///
///		// Call a member function
///		A a;
///		Scheduler::addTask(&A::test, &a, "Class A");
///
///
///		// Using a task
///		Task<float> task = Scheduler::addTask(add, 5.0f, 4.0f);
///
///
///		// Wait for all tasks to finish
///		Scheduler::finish();
///		// Join all worker threads
///		Scheduler::stop();
///
///		// Check the results
///		if (task.isFinished())
///			// This should print 9.0
///			std::cout << task.getResult() << '\n';
///
///		return 0;
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////