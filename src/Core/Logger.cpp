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

std::mutex Logger::m_mutex;
std::mutex Logger::m_threadMutex;

///////////////////////////////////////////////////////////

bool Logger::init(const std::string& fname)
{
	// Open log file
	m_file.open(fname);

	// Print header
	std::string header = " Time                     |  Thread name:ID        |  Message\n";
	header += "---------------------------------------------------------------------------------------------\n";

#ifndef NDEBUG
	std::cerr << header;
#endif

	if (m_file.is_open())
		m_file << header;

	return m_file.is_open();
}

///////////////////////////////////////////////////////////

void Logger::log(Logger::Verbosity verbosity, const std::string& msg)
{
	std::thread::id id = std::this_thread::get_id();
	if (m_scheduler)
		m_scheduler->addTask(m_priority, &Logger::logMsg, verbosity, msg, id);
	else
		logMsg(verbosity, msg, id);
}

void Logger::logMsg(Logger::Verbosity verbosity, const std::string& msg, std::thread::id threadId)
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
		std::string& name = m_threadNames[std::this_thread::get_id()];

		// Create a new name for the thread if one doesn't exist
		if (!name.size())
			name = "Thread #" + std::to_string(m_threadNames.size());

		threadName = name;
	}

	// Construct the thread label
	{
		Uint32 start = ss.tellp();
		ss << threadName << ':';
		Uint32 size = (Uint32)ss.tellp() - start;

		const Uint32 threadNameWidth = 20;
		Uint32 width = threadNameWidth - size;
		ss << std::setw(width) << std::left << std::this_thread::get_id() << "] | ";
	}

	// Create verbosity label
	if (verbosity == Info)
		ss << "[INFO]    - ";
	else if (verbosity == Warning)
		ss << "[WARNING] - ";
	else if (verbosity == Error)
		ss << "[ERROR]   - ";
	else
		ss << "[FATAL]   - ";

	ss << msg << '\n';

	// Create log line
	std::string line = ss.str();

	// Lock mutex
	std::lock_guard<std::mutex> lock(m_mutex);

#ifndef NDEBUG
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set console color
	if (verbosity == Warning)
		SetConsoleTextAttribute(hConsole, 6);
	else if (verbosity == Error)
		SetConsoleTextAttribute(hConsole, 12);
	else if (verbosity == Fatal)
		SetConsoleTextAttribute(hConsole, 4);

	std::cerr << line;

	// Reset color afterwards
	SetConsoleTextAttribute(hConsole, 7);
#endif
#endif

	if (m_file.is_open())
		m_file << line;
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