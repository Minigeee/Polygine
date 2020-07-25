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

bool Logger::m_shouldFlush[5] = { true, true, false, false, false };

///////////////////////////////////////////////////////////

bool Logger::init(const std::string& fname)
{
	// Open log file
	m_file.open(fname);

	// Print header
	std::string header = " Time                     |  Thread name      |  Message\n";
	header += "---------------------------------------------------------------------------------------------\n";

#ifndef NDEBUG
	std::cerr << header;
#endif

	if (m_file.is_open())
		m_file << header;

	return m_file.is_open();
}

///////////////////////////////////////////////////////////

void Logger::log(Logger::MsgType type, const std::string& msg)
{
	std::thread::id id = std::this_thread::get_id();

	if (type == Error || type == Fatal)
		// Force error and fatal messages to be synchronous
		logMsg(type, msg, id);

	else
	{
		// Otherwise, use the scheduler if possible
		if (m_scheduler)
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);

			// Add a log message object to the queue if doing async
			m_msgQueue.push(LogMsg({ type, msg, id }));

			// Only add another async function if there is only 1 message in the queue
			if (m_msgQueue.size() == 1)
				m_scheduler->addTask(m_priority, &Logger::logAsync);
		}
		else
			logMsg(type, msg, id);
	}
}

void Logger::logMsg(Logger::MsgType type, const std::string& msg, std::thread::id threadId)
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

///////////////////////////////////////////////////////////

}