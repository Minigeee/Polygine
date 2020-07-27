#include <poly/Core/Logger.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef WIN32
#include <Windows.h>
#endif

namespace poly
{

std::ofstream Logger::m_file;

Scheduler* Logger::m_scheduler = 0;
Scheduler::Priority Logger::m_priority = Scheduler::Low;
std::unordered_map<std::thread::id, std::string> Logger::m_threadNames;
std::queue<Logger::LogMsg> Logger::m_msgQueue;

std::mutex Logger::m_mutex;
std::mutex Logger::m_threadMutex;
std::mutex Logger::m_queueMutex;
std::atomic<bool> Logger::m_taskExists = false;

bool Logger::m_shouldFlush[5] = { true, true, false, false, false };

///////////////////////////////////////////////////////////

bool Logger::init(const std::string& fname)
{
	// Open log file
	m_file.open(fname);

	// Print header
#ifndef NDEBUG
	std::string header = " Time                     |  Thread name      |  File:Line             |  Message\n";
#else
	std::string header = " Time                     |  Thread name      |  Message\n";
#endif
	header += "-------------------------------------------------------------------------------------------------------\n";

#ifndef NDEBUG
	std::cerr << header;
#endif

	if (m_file.is_open())
		m_file << header;

	return m_file.is_open();
}

///////////////////////////////////////////////////////////

void Logger::log(Logger::MsgType type, const std::string& msg, const std::string& loc)
{
	std::thread::id id = std::this_thread::get_id();

	if (type == Error || type == Fatal)
		// Force error and fatal messages to be synchronous
		logMsg(type, msg, id, loc);

	else
	{
		// Otherwise, use the scheduler if possible
		if (m_scheduler)
		{
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);

				// Add a log message object to the queue if doing async
				m_msgQueue.push(LogMsg({ type, msg, loc, id }));
			}

			// Only add another async function if there is only 1 message in the queue
			if (!m_taskExists)
			{
				m_taskExists = true;
				m_scheduler->addTask(m_priority, &Logger::logAsync);
			}
		}
		else
			logMsg(type, msg, id, loc);
	}
}

void Logger::logMsg(Logger::MsgType type, const std::string& msg, std::thread::id threadId, const std::string& loc)
{
	std::ostringstream ss;

	// Get date and time
	time_t t;
	struct tm* timeinfo;
	char buffer[80];

	t = time(NULL);
	timeinfo = gmtime(&t);
	strftime(buffer + 1, sizeof(buffer) - 1, "%d-%m-%Y %H:%M:%S", timeinfo);
	buffer[0] = '[';

	ss << buffer << " UTC] | [";

	// Get thread name
	std::string threadName;
	{
		std::lock_guard<std::mutex> lock(m_threadMutex);
		std::string& name = m_threadNames[threadId];

		// Create a new name for the thread if one doesn't exist
		if (!name.size())
			name = "Thread #" + std::to_string(m_threadNames.size());

		threadName = name;
	}

	// Construct the thread label
	ss << std::setw(15) << std::left << threadName << "] | ";

#ifndef NDEBUG

	if (loc.size())
	{
		// Make sure only the file name is included, not the path
		Uint32 pos = loc.find_last_of("/\\");
		std::string srcFile = loc.substr(pos + 1, loc.size() - pos - 1);
		ss << '[' << std::setw(20) << std::left << srcFile << "] | ";
	}

#endif

	// Create type label
	if (type == Info)
		ss << "[INFO]    - ";
	else if (type == Warning)
		ss << "[WARNING] - ";
	else if (type == Error)
		ss << "[ERROR]   - ";
	else if (type == Fatal)
		ss << "[FATAL]   - ";
	else
		ss << "[DEBUG]   - ";

	ss << msg << '\n';

	// Create log line
	std::string line = ss.str();

	// Lock mutex
	std::lock_guard<std::mutex> lock(m_mutex);

#ifndef NDEBUG
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set console color
	if (type == Warning)
		SetConsoleTextAttribute(hConsole, 6);
	else if (type == Error)
		SetConsoleTextAttribute(hConsole, 12);
	else if (type == Fatal)
		SetConsoleTextAttribute(hConsole, 4);
	else if (type == Debug)
		SetConsoleTextAttribute(hConsole, 10);

	std::cerr << line;

	// Reset color afterwards
	SetConsoleTextAttribute(hConsole, 7);
#else

	// TODO : Colored output for linux/macos

#endif
#endif

	if (m_file.is_open())
	{
		m_file << line;

		// Flush depending on message type
		if (m_shouldFlush[type])
			std::flush(m_file);
	}
}

void Logger::logAsync()
{
	m_queueMutex.lock();

	// Loop through the queue and log everything
	while (!m_msgQueue.empty())
	{
		LogMsg msg = std::move(m_msgQueue.front());
		m_msgQueue.pop();
		m_queueMutex.unlock();

		// Use synchronous log
		logMsg(msg.m_type, msg.m_msg, msg.m_threadId);

		m_queueMutex.lock();
	}

	// Mark that no tasks currently exist to handle async messages
	// so next time a message is queued, a new task is created
	m_taskExists = false;

	m_queueMutex.unlock();
}

///////////////////////////////////////////////////////////

void Logger::setThreadName(const std::string& name)
{
	// Map this thread's id to the name
	std::lock_guard<std::mutex> lock(m_threadMutex);
	m_threadNames[std::this_thread::get_id()] = name;
}

void Logger::setScheduler(Scheduler* scheduler, Scheduler::Priority priority)
{
	m_scheduler = scheduler;
	m_priority = priority;
}

void Logger::setFlush(Logger::MsgType type, bool flush)
{
	m_shouldFlush[type] = flush;
}

///////////////////////////////////////////////////////////

namespace priv
{

// Code source: https://stackoverflow.com/questions/69738/c-how-to-get-fprintf-results-as-a-stdstring-w-o-sprintf#69911

std::string vformat(const char* fmt, va_list ap)
{
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.  Be prepared to allocate dynamically if it doesn't fit.
	size_t size = 1024;
	char stackbuf[1024];
	std::vector<char> dynamicbuf;
	char* buf = &stackbuf[0];
	va_list ap_copy;

	while (1) {
		// Try to vsnprintf into our buffer.
		va_copy(ap_copy, ap);
		int needed = vsnprintf(buf, size, fmt, ap);
		va_end(ap_copy);

		// NB. C99 (which modern Linux and OS X follow) says vsnprintf
		// failure returns the length it would have needed.  But older
		// glibc and current Windows return -1 for failure, i.e., not
		// telling us how much was needed.

		if (needed <= (int)size && needed >= 0) {
			// It fit fine so we're done.
			return std::string(buf, (size_t)needed);
		}

		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So try again using a dynamic buffer.  This
		// doesn't happen very often if we chose our initial size well.
		size = (needed > 0) ? (needed + 1) : (size * 2);
		dynamicbuf.resize(size);
		buf = &dynamicbuf[0];
	}
}

std::string format(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	std::string buf = vformat(fmt, ap);
	va_end(ap);
	return buf;
}

}

}