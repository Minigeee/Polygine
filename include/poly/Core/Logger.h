#ifndef POLY_LOGGER_H
#define POLY_LOGGER_H

#include <poly/Core/Scheduler.h>

#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

namespace poly
{

class Logger
{
public:
	enum Verbosity
	{
		Fatal,
		Error,
		Warning,
		Info
	};

public:
	static bool init(const std::string& fname);

	static void setThreadName(const std::string& name);

	static void log(Verbosity verbosity, const std::string& msg);

	static void setScheduler(Scheduler* scheduler, Scheduler::Priority priority = Scheduler::Low);

private:
	static void logMsg(Verbosity verbosity, const std::string& msg, std::thread::id threadId);

	static std::ofstream m_file;

	static Scheduler* m_scheduler;
	static Scheduler::Priority m_priority;
	static std::unordered_map<std::thread::id, std::string> m_threadNames;

	static std::mutex m_mutex;
	static std::mutex m_threadMutex;
};

#define LOG(msg)			poly::Logger::log(poly::Logger::Info, msg)
#define LOG_WARNING(msg)	poly::Logger::log(poly::Logger::Warning, msg)
#define LOG_ERROR(msg)		poly::Logger::log(poly::Logger::Error, msg)
#define LOG_FATAL(msg)		poly::Logger::log(poly::Logger::Fatal, msg)

}

#endif