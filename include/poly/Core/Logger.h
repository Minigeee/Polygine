#ifndef POLY_LOGGER_H
#define POLY_LOGGER_H

#include <poly/Core/Macros.h>
#include <poly/Core/Scheduler.h>

#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

namespace poly
{

namespace priv
{

std::string vformat(const char* fmt, va_list ap);
std::string format(const char* fmt, ...);

}

///////////////////////////////////////////////////////////
/// \brief A class used for logging messages
///
///////////////////////////////////////////////////////////
class Logger
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Types of log messages
	///
	///////////////////////////////////////////////////////////
	enum MsgType
	{
		Fatal,		//!< Fatal error message type
		Error,		//!< Error message type
		Warning,	//!< Warning message type
		Info,		//!< Info message type
		Debug,		//!< Debug message type
		None		//!< No message tag (for custom tags)
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Initialize the logger
	///
	/// This function is not necessary to call if logging to the
	/// console is enough, but it is required to write to a log file.
	/// This function sets up the log file, and prints an infomational
	/// header with column titles.
	///
	/// \param fname The name of the log file to write to
	///
	///////////////////////////////////////////////////////////
	static bool init(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Set a custom name for a thread
	///
	/// Set a custom name for the current thread to help with
	/// debugging. The logger will print the name of the thread
	/// the log message was sent on. If there is no custom name set
	/// for the thread, it will default to "Thread #N".
	///
	/// Note that this function must be called from the target thread
	/// for it to work.
	///
	/// \param name Custom thread name
	///
	///////////////////////////////////////////////////////////
	static void setThreadName(const std::string& name);

	///////////////////////////////////////////////////////////
	/// \brief Log a message
	///
	/// It is possible to use this function directly to log,
	/// but it is recommended to use one of the macros:
	///
	/// \li #LOG
	/// \li #LOG_WARNING
	/// \li #LOG_ERROR
	/// \li #LOG_FATAL
	/// \li #LOG_DEBUG
	///
	/// \param type The message #Type
	/// \param msg The message to log
	/// \param loc The location string indicating file name and line
	///
	///////////////////////////////////////////////////////////
	static void log(MsgType type, const std::string& msg, const std::string& loc = "");

	///////////////////////////////////////////////////////////
	/// \brief Set whether or not the logger should use a scheduler for asyncrhonous logging
	///
	/// Whenever a log message is sent, it will be passed as a
	/// task to the scheduler instead. By default, the priority
	/// level of log message tasks is low, but a custom priority
	/// level can be specified in this function.
	///
	/// A problem with this asynchronous logging method is that
	/// log message tasks may get delayed for a while, and some
	/// messages may be lost if they aren't logged before the
	/// program crashes. This is why \link MsgType::Error \endlink
	/// and \link MsgType::Fatal \endlink are forced to be
	/// synchronous, even if a scheduler has been provided.
	/// It will not guarantee that the messages are logged before
	/// a crash, but it lessens the risk.
	///
	/// Remember to use Scheduler::finish() to ensure that all log
	/// messages are printed before stopping the program.
	///
	/// \param scheduler A pointer to a scheduler to use
	/// \param priority Optional priority value to assign to log tasks
	///
	///////////////////////////////////////////////////////////
	static void setUseScheduler(bool use, Scheduler::Priority priority = Scheduler::Low);

	///////////////////////////////////////////////////////////
	/// \brief Set if certain message types should flush their file output
	///
	/// Flushing file output will force log messages to write to
	/// the file stream. In the event that a crash occurs, all log
	/// messages that have already been written will be kept
	/// in the log file. Messages that don't get flushed may not
	/// make it into the file.
	///
	/// By default, only \link MsgType::Error \endlink
	/// and \link MsgType::Fatal \endlink have their output flushed.
	///
	/// \param type The #MsgType to modify
	/// \param shouldFlush Set to true if the specified message type should flush
	///
	///////////////////////////////////////////////////////////
	static void setFlush(MsgType type, bool shouldFlush);

private:
	struct LogMsg
	{
		MsgType m_type;
		std::string m_msg;
		std::string m_loc;
		std::thread::id m_threadId;
	};

	///////////////////////////////////////////////////////////
	/// \brief The function that does the synchronous logging
	///
	///////////////////////////////////////////////////////////
	static void logAsync();

	///////////////////////////////////////////////////////////
	/// \brief The function that does the synchronous logging
	///
	///////////////////////////////////////////////////////////
	static void logMsg(MsgType type, const std::string& msg, std::thread::id threadId, const std::string& loc = "");


	static std::ofstream m_file;			//!< The file stream to write the log to

	static bool m_useScheduler;				//!< Scheduler for asynchronous logging
	static Scheduler::Priority m_priority;	//!< Priority level to give logging tasks
	static HashMap<std::thread::id, std::string> m_threadNames;	//!< Map of thread IDs to names for custom thread names
	static std::queue<LogMsg> m_msgQueue;	//!< The message queue used for asynchronous messages

	static bool m_shouldFlush[5];			//!< Array that determines which message types should flush output

	static std::mutex m_mutex;				//!< Mutex to protect log outputs
	static std::mutex m_threadMutex;		//!< Mutex to protect the thread names map
	static std::mutex m_queueMutex;			//!< Mutex to protect the log queue
	static std::atomic<bool> m_taskExists;	//!< Flag used to ensure only 1 logging thread exists at a time
};

///////////////////////////////////////////////////////////
/// \brief Log a \link MsgType::Info \endlink message
///
/// Messages of this type show up in white in the console.
/// If a scheduler is provided, then messages logged with
/// this macro will be asynchronous.
///
/// This macro uses printf style formatting, so all arguments
/// after the formatting string will be used to create the string.
///
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define LOG(msg, ...) poly::Logger::log(poly::Logger::Info, poly::priv::format(msg, __VA_ARGS__), std::string(__FILE__) + ':' + std::to_string(__LINE__))
#else
#define LOG(msg, ...) poly::Logger::log(poly::Logger::Info, poly::priv::format(msg, __VA_ARGS__))
#endif

///////////////////////////////////////////////////////////
/// \brief Log a \link MsgType::Warning \endlink message
///
/// Messages of this type show up in yellow in the console.
/// If a scheduler is provided, then messages logged with
/// this macro will be asynchronous.
///
/// This macro uses printf style formatting, so all arguments
/// after the formatting string will be used to create the string.
///
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define LOG_WARNING(msg, ...) poly::Logger::log(poly::Logger::Warning, poly::priv::format(msg, __VA_ARGS__), std::string(__FILE__) + ':' + std::to_string(__LINE__))
#else
#define LOG_WARNING(msg, ...) poly::Logger::log(poly::Logger::Warning, poly::priv::format(msg, __VA_ARGS__))
#endif

///////////////////////////////////////////////////////////
/// \brief Log a \link MsgType::Error \endlink message
///
/// Messages of this type show up in light red in the console.
/// These messages will always be synchronous, even if a
/// scheduler is provided. This is to increase the change
/// that the message is logged in the case of a crash.
///
/// This macro uses printf style formatting, so all arguments
/// after the formatting string will be used to create the string.
///
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define LOG_ERROR(msg, ...) poly::Logger::log(poly::Logger::Error, poly::priv::format(msg, __VA_ARGS__), std::string(__FILE__) + ':' + std::to_string(__LINE__))
#else
#define LOG_ERROR(msg, ...) poly::Logger::log(poly::Logger::Error, poly::priv::format(msg, __VA_ARGS__))
#endif

///////////////////////////////////////////////////////////
/// \brief Log a \link Logger::MsgType::Fatal \endlink message
///
/// Messages of this type show up in red in the console.
/// These messages will always be synchronous, even if a
/// scheduler is provided. This is to increase the change
/// that the message is logged in the case of a crash.
///
/// This macro uses printf style formatting, so all arguments
/// after the formatting string will be used to create the string.
///
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define LOG_FATAL(msg, ...) poly::Logger::log(poly::Logger::Fatal, poly::priv::format(msg, __VA_ARGS__), std::string(__FILE__) + ':' + std::to_string(__LINE__))
#else
#define LOG_FATAL(msg, ...) poly::Logger::log(poly::Logger::Fatal, poly::priv::format(msg, __VA_ARGS__))
#endif

///////////////////////////////////////////////////////////
/// \brief Log a \link MsgType::Debug \endlink message
///
/// Messages of this type show up in green in the console.
/// If a scheduler is provided, then messages logged with
/// this macro will be asynchronous.
///
/// When compiled in release mode, debug messages won't
/// be logged if this macro is used.
///
/// This macro uses printf style formatting, so all arguments
/// after the formatting string will be used to create the string.
///
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define LOG_DEBUG(msg, ...) poly::Logger::log(poly::Logger::Debug, poly::priv::format(msg, __VA_ARGS__), std::string(__FILE__) + ':' + std::to_string(__LINE__))
#else
#define LOG_DEBUG(msg, ...)
#endif

///////////////////////////////////////////////////////////
/// \brief Check a condition and log a \link MsgType::Error \endlink message if the condition is false
///
/// This macro works in the same wasy as the standard assert(),
/// but it logs using the logger system instead. Messages
/// are logged with the \link MsgType::Error \endlink level,
/// and abort() is called right after the condition fails.
///
/// This macro also uses printf style formatting like all
/// the other logging functions.
///
/// This macro is disabled in release mode.
///
/// \param cond The condition to check
/// \param msg The message to log (the formatting string)
///
///////////////////////////////////////////////////////////
#ifndef NDEBUG
#define ASSERT(cond, msg, ...) if (!(cond)) { LOG_ERROR("%s: %s", STR(cond), poly::priv::format(msg, __VA_ARGS__).c_str()); abort(); }
#else
#define ASSERT(cond, msg, ...)
#endif

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Logger
/// \ingroup Core
///
/// Logger is a static utility class that provides logging
/// functionality.
///
/// Some of its features are:
/// \li Date and timestamps
/// \li Custom thread names
/// \li Message types: Info, Warning, Error, Fatal
/// \li Asynchronous logging
/// \li Colored console output
/// \li File name and line number
///
/// In order to log to a file, init() must be called and
/// passed a file path. To use asynchronous logging,
/// a Scheduler must be created, and a pointer to it must be passed
/// to setScheduler().
///
/// While it is possible to log messages using the log() function,
/// using one of the following macros is recommended:
///
/// \li #LOG
/// \li #LOG_WARNING
/// \li #LOG_ERROR
/// \li #LOG_FATAL
/// \li #LOG_DEBUG
///
/// Note that this is a very basic logging class that exists
/// for convenience. There are other libraries that are much
/// more thorough, effecient, and reliable than this class
/// if more serious logging is needed.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Logger::init("test.log");
///
/// // Set up asyncrhonous logging
/// Scheduler scheduler;
/// Logger::setScheduler(&scheduler);
///
/// // Log some stuff
/// LOG("Hello World!\n");
///
/// // Log a warning
/// LOG_WARNING("You are getting hungry");
///
/// // Log some useless stuff
/// LOG("There is a bird outside your window");
///
/// // Log an error
/// // Even though async logging is setup, this will forced to be synchronous
/// LOG_ERROR("The power has gone out");
///
/// // Log fatal error
/// // Even though async logging is setup, this will forced to be synchronous
/// LOG_FATAL("You have died");
///
/// // Printf style formatting
/// LOG("Pi is: %.5f", 3.141592f);
///
/// scheduler.finish();
///
/// \endcode
///
///////////////////////////////////////////////////////////