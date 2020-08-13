#ifndef POLY_PROFILER_H
#define POLY_PROFILER_H

#include <poly/Core/Clock.h>
#include <poly/Core/DataTypes.h>
#include <poly/Core/Macros.h>

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A struct containing profiler data gathered from markers
///
/// See Profiler for more detail.
///
///////////////////////////////////////////////////////////
struct ProfilerData
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor for data
	///
	///////////////////////////////////////////////////////////
	ProfilerData();

	///////////////////////////////////////////////////////////
	/// \brief Calculate the mean runtime of the profiler data
	///
	/// Calculates the mean of all data points gathered by
	/// a profiler marker. See the profiler usage example for
	/// more info on how to use this function.
	///
	/// \return Returns the mean run time measured by profiler markers
	///
	///////////////////////////////////////////////////////////
	Time mean() const;

	///////////////////////////////////////////////////////////
	/// \brief Calculate the standard deviation of the profiler data
	///
	/// Calculates the standard deviation of all data points gathered by
	/// a profiler marker. See the profiler usage example for
	/// more info on how to use this function.
	///
	/// \return Returns the standard deviation of a profiler marker data
	///
	///////////////////////////////////////////////////////////
	Time stdDev() const;

	std::string m_label;			//!< The label of the profiler marker this data was retrieved from
	std::string m_func;				//!< The function name that the profiler marker was activated from
	std::vector<Time> m_interval;	//!< The current interval, used to calculate the interval average. It contains the latest 10 iterations the profiler marker has recorded.
	std::vector<Time> m_averages;	//!< The list of interval averages, where each value is the average of a certain interval of values
	Uint32 m_numRuns;				//!< The number of times the profiler marker that measures execution time has been run
};

///////////////////////////////////////////////////////////
/// \brief A class used to measure execution time of a section of code
///
/// See Profiler for more detail.
///
///////////////////////////////////////////////////////////
class ProfilerMarker
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ProfilerMarker();

	///////////////////////////////////////////////////////////
	/// \brief Construct the marker with a label and the name of its containing function
	///
	///////////////////////////////////////////////////////////
	ProfilerMarker(const std::string& label, const std::string& func);

	///////////////////////////////////////////////////////////
	/// \brief Automatically stops measuring execution time if it hasn't already
	///
	///////////////////////////////////////////////////////////
	~ProfilerMarker();

	///////////////////////////////////////////////////////////
	/// \brief Start measuring the execution time of a section of code
	///
	/// \see stop
	///
	///////////////////////////////////////////////////////////
	void start();

	///////////////////////////////////////////////////////////
	/// \brief Stop measuring the execution time of a section of code
	///
	/// This function calculates the elapsed time since start()
	/// was called and temporarily stores it. Then it automatically
	/// calls Profiler::addMarker() to record the data.
	///
	/// \see start
	///
	///////////////////////////////////////////////////////////
	void stop();

	///////////////////////////////////////////////////////////
	/// \brief Get the amount of elapsed time taken for a section of code to execute
	///
	/// This function gets the amount of time between when start()
	/// was called and when stop() was called.
	///
	/// This will return 0 if stop() hasn't been called yet.
	///
	/// \return Time object containing elapsed run time
	///
	///////////////////////////////////////////////////////////
	const Time& getElapsedTime() const;

	///////////////////////////////////////////////////////////
	/// \brief Gets the label of the marker
	///
	/// \return A string containing the label of the marker
	///
	///////////////////////////////////////////////////////////
	const std::string& getLabel() const;

	///////////////////////////////////////////////////////////
	/// \brief Gets the name of the function containing marker
	///
	/// \return A string containing the name of the function containing marker
	///
	///////////////////////////////////////////////////////////
	const std::string& getFunc() const;

private:
	Clock m_clock;			//!< The clock used to measure elapsed time
	Time m_elapsedTime;		//!< Elapsed execution time
	std::string m_label;	//!< Label of the marker
	std::string m_func;		//!< Name of the function containing the marker
	bool m_isRunning;		//!< True if time is still being measured
};

///////////////////////////////////////////////////////////
/// \brief A class used for storing execution times measured by markers
///
///////////////////////////////////////////////////////////
class Profiler
{
public:
	///////////////////////////////////////////////////////////
	/// \brief A static function that records the time data measured by a marker
	///
	/// This function will keep track of the data the marker measures
	/// over many iterations. Each time the same marker is added,
	/// its data is added to the ProfilerData list, which can be used
	/// to calclate various statistics, such as the mean and
	/// standard deviation.
	///
	/// The data that is recorded can be accessed by getData()
	///
	/// \param marker The ProfilerMarker to record
	///
	/// \see getData
	///
	///////////////////////////////////////////////////////////
	static void addMarker(const ProfilerMarker& marker);

	///////////////////////////////////////////////////////////
	/// \brief A static function that retrieves the data recorded by a certain marker
	///
	/// Data recorded by markers can be accessed using the marker's
	/// function name (required) and its label (optional). Both
	/// the function containing the marker and its label must be known
	/// in order to retrieve data by that marker.
	///
	/// \return A ProfilerData that contains data on the specified marker
	///
	/// \see addMarker
	///
	///////////////////////////////////////////////////////////
	static const ProfilerData& getData(const std::string& func, const std::string& label = "");

private:
	static std::unordered_map<std::string, ProfilerData> m_data; //!< Map used to store the data
	static std::mutex m_mutex;									 //!< Mutex to protect access to profiler data
};

#ifdef ENABLE_PROFILING

///////////////////////////////////////////////////////////
/// \brief Convenience macro used to profile an entire function rather than a section of code
///
/// This will create a new profiler marker with an empty label,
/// and call its start() function.
///
/// \see poly::Profiler
///
///////////////////////////////////////////////////////////
#define START_PROFILING_FUNC \
poly::ProfilerMarker Profiler_("", __FUNCTION__); \
Profiler_.start();

///////////////////////////////////////////////////////////
/// \brief Convenience macro used to start profiling a section of code
///
/// This will create a new profiler marker with the specified label,
/// and call its start() function.
///
/// \see poly::Profiler
///
///////////////////////////////////////////////////////////
#define START_PROFILING(label) \
poly::ProfilerMarker CONCAT(Profiler_, label)(STR(label), __FUNCTION__); \
CONCAT(Profiler_, label).start();

///////////////////////////////////////////////////////////
/// \brief Convenience macro used to stop profiling a section of code
///
/// This will call its stop() function.
///
/// \see poly::Profiler
///
///////////////////////////////////////////////////////////
#define STOP_PROFILING(label) CONCAT(Profiler_, label).stop();

#else
#define START_PROFILING_FUNC
#define START_PROFILING(x)
#define STOP_PROFILING(x)
#endif

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Profiler
/// \ingroup Core
///
/// Profiler and its helper classes are used to perform
/// simple profiling of code. They can be used to count the
/// number of times a certain section is run, as well as the
/// time it takes to run that section.
///
/// To use the profiler system, the ProfilerMarker must be used
/// to measure the execution time of a certain section of code.
/// After done measuring, the execution time will automatically
/// be stored in the profiler, where it can be accessed using
/// getData().
///
/// Data is stored in interval averages, where each data value
/// is seperated into intervals of 10. The average value of each
/// interval is calculated and stored. The main purpose of storing
/// the average of each interval, rather than storing each value
/// individually, is that it makes the data less noisy as
/// well as taking up less space. And the interval is small
/// enough to where detecting outliers would be possible.
///
/// Though it is possible to use the profiler system manually,
/// it is easier to use the predefined macros:
/// \li #START_PROFILING_FUNC
/// \li #START_PROFILING(label)
/// \li #STOP_PROFILING(label)
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// void test()
/// {
///		// This will create a ProfilerMarker with an empty label
///		// It is normally used when you want to profile an entire function,
///		// rather than a small section
///		START_PROFILING_FUNC;
///
///
///		// Use this macro to profile certain sections rather than an entire function
///		// Note that the label parameter doesn't include qutation marks
///		START_PROFILING(Loop1);
///
///		std::vector<int> v1;
///		for (int i = 0; i < 10000; ++i)
///			v1.push_back(i);
///
///		// To stop profiling, use the stop macro
///		STOP_PROFILING(Loop1);
///
///
///		// Start profiling a different section with a different label
///		START_PROFILING(Loop2);
///
///		std::vector<int> v2;
///		for (int i = 0; i < 10000; ++i)
///			v1.push_back(2);
///
///		// The stop macro doesn't have to be called if the function will end right after
///		// The stop macro just calls the profile marker's stop() function,
///		// but going out of scope will call the marker's destructor, which achieves the same result
/// }
///
/// int main()
/// {
///		// Call the function
///		test();
///
///		// Now, profiler data is accessible
///		const ProfilerData& fData = Profiler::getData("test");
///		const ProfilerData& l1Data = Profiler::getData("test", "Loop1");
///		const ProfilerData& l2Data = Profiler::getData("test", "Loop2");
///
///		std::cout << fData.mean().toSeconds() << "s, " << fData.stdDev().toSeconds() << "s\n";
///		std::cout << l1Data.mean().toSeconds() << "s, " << l1Data.stdDev().toSeconds() << "s\n";
///		std::cout << l2Data.mean().toSeconds() << "s, " << l2Data.stdDev().toSeconds() << "s\n";
///
///		return 0;
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////