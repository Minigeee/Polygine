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
	/// Functions that have return values will not be retrievable,
	/// so if some result is needed from the task, use another
	/// method to retrieve the results (i.e. setting a member variable).
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
	///////////////////////////////////////////////////////////
	template <typename F, typename... Args>
	void addTask(F&& func, Args&&... args);

	///////////////////////////////////////////////////////////
	/// \brief Add a task function with a certain priority for the scheduler to execute
	///
	/// This function adds the specified function to a queue
	/// for the worker threads to execute when available.
	/// Functions that have return values will not be retrievable,
	/// so if some result is needed from the task, use another
	/// method to retrieve the results (i.e. setting a member variable).
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
	///////////////////////////////////////////////////////////

	template <typename F, typename... Args>
	void addTask(Priority priority, F&& func, Args&&... args);

	///////////////////////////////////////////////////////////
	/// \brief Wait for all tasks in the queue to finish
	///
	/// This function will block the calling thread until all
	/// current tasks and tasks in the queue have finished.
	///
	///////////////////////////////////////////////////////////
	void finish();

	///////////////////////////////////////////////////////////
	/// \brief Clears the task queue and stops all worker threads
	///
	/// This function clears the task queue to prevent any extra
	/// tasks from being executed. Then it blocks the calling
	/// thread until all tasks that are currently being run are
	/// and have joined the current thread.
	///
	///////////////////////////////////////////////////////////
	void stop();

private:
	///////////////////////////////////////////////////////////
	/// \brief The loop that worker threads use
	///
	///////////////////////////////////////////////////////////
	void workerLoop(Uint32 id);

	std::queue<std::function<void()>> m_queue[3];	//!< The task queue
	std::vector<std::thread> m_threads;				//!< The list of worker threads
	std::atomic<Uint32> m_numBusy;					//!< The number of busy threads
	std::atomic<bool> m_shouldStop;					//!< True if stop() has been called

	std::mutex m_mutex;								//!< Mutex to protect queue and for condition variables
	std::condition_variable m_scv;					//!< The condition variable used to notify new tasks
	std::condition_variable m_fcv;					//!< The condition variable used to notify finishing tasks
};

#include <poly/Core/Scheduler.inl>

}

#endif

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
/// int main()
/// {
///		// The constructor will create a certain number of threads
///		Scheduler scheduler;
///
///		// These tasks will be added into the high priority queue
///		scheduler.addTask(test, "World");
///		scheduler.addTask(test, "ABC");
///
///
///		// Add a low priority task
///		scheduler.addTask(Scheduler::Low, test, "Low");
///		// Even though this task was added after the low priority, it will execute first
///		scheduler.addTask(Scheduler::Medium, test, "Medium");
///
///
///		// Call a member function
///		A a;
///		scheduler.addTask(&A::test, &a, "Class A");
///
///
///		// Wait for all tasks to finish
///		scheduler.finish();
///		// Join all worker threads
///		scheduler.stop();
///
///		return 0;
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////