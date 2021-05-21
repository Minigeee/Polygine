#include <poly/Core/Profiler.h>

namespace poly
{

HashMap<std::string, ProfilerData> Profiler::m_data;
std::mutex Profiler::m_mutex;


///////////////////////////////////////////////////////////
ProfilerData::ProfilerData() :
	m_numRuns		(0)
{ }


///////////////////////////////////////////////////////////
Time ProfilerData::mean() const
{
	// Do calculations with double and it seconds
	double avg = 0.0;

	// First, calculate the average of remaining times in interval
	for (Uint32 i = 0; i < m_interval.size(); ++i)
		avg += (double)m_interval[i].toSeconds();
	avg /= (double)m_interval.size();

	// Calculate average with the rest of values
	for (Uint32 i = 0; i < m_averages.size(); ++i)
		avg += (double)m_averages[i].toSeconds();
	avg /= (double)m_averages.size() + (m_interval.size() ? 1.0 : 0.0);

	return Time::fromSeconds((float)avg);
}


///////////////////////////////////////////////////////////
Time ProfilerData::stdDev() const
{
	// First, calculate the average of remaining times in interval
	double intervalAvg = 0.0;
	for (Uint32 i = 0; i < m_interval.size(); ++i)
		intervalAvg += (double)m_interval[i].toSeconds();
	intervalAvg /= (double)m_interval.size();

	// Calculate average with the rest of values
	double avg = intervalAvg;
	for (Uint32 i = 0; i < m_averages.size(); ++i)
		avg += (double)m_averages[i].toSeconds();
	avg /= (double)m_averages.size() + (m_interval.size() ? 1.0 : 0.0);

	// Calculate standard deviation
	double diff = intervalAvg - avg;
	double stdDev = diff * diff;
	for (Uint32 i = 0; i < m_averages.size(); ++i)
	{
		diff = (double)m_averages[i].toSeconds() - avg;
		stdDev += diff * diff;
	}
	stdDev /= (double)m_averages.size() + (m_interval.size() ? 1.0 : 0.0);

	return Time::fromSeconds((float)sqrt(stdDev));
}


///////////////////////////////////////////////////////////
const std::vector<Time>& ProfilerData::getAverages() const
{
	return m_averages;
}


///////////////////////////////////////////////////////////
ProfilerMarker::ProfilerMarker() :
	m_elapsedTime	(0),
	m_label			(0),
	m_func			(0),
	m_isRunning		(false)
{

}


///////////////////////////////////////////////////////////
ProfilerMarker::ProfilerMarker(const std::string& label, const std::string& func) :
	m_elapsedTime	(0),
	m_label			(label),
	m_func			(func),
	m_isRunning		(false)
{

}


///////////////////////////////////////////////////////////
ProfilerMarker::~ProfilerMarker()
{
	stop();
}


///////////////////////////////////////////////////////////
void ProfilerMarker::start()
{
	// Start the clock
	m_clock.restart();
	m_isRunning = true;
}


///////////////////////////////////////////////////////////
void ProfilerMarker::stop()
{
	if (m_isRunning)
	{
		// Record the time
		m_elapsedTime = m_clock.getElapsedTime();

		// Add self to main profiler
		Profiler::addMarker(*this);

		m_isRunning = false;
	}
}


///////////////////////////////////////////////////////////
const Time& ProfilerMarker::getElapsedTime() const
{
	return m_elapsedTime;
}


///////////////////////////////////////////////////////////
const std::string& ProfilerMarker::getLabel() const
{
	return m_label;
}


///////////////////////////////////////////////////////////
const std::string& ProfilerMarker::getFunc() const
{
	return m_func;
}


///////////////////////////////////////////////////////////
void Profiler::addMarker(const ProfilerMarker& marker)
{
	ProfilerData* data;

	// Construct marker name
	std::string name = marker.getLabel() + ':' + marker.getFunc();

	// Data is accessed from here on
	std::lock_guard<std::mutex> lock(m_mutex);

	// Find the entry for the profiler marker
	auto it = m_data.find(name);
	if (it == m_data.end())
	{
		// Create an entry if profiler marker hasn't been entered yet
		ProfilerData newEntry;
		newEntry.m_label = marker.getLabel();
		newEntry.m_func = marker.getFunc();
		newEntry.m_numRuns = 0;

		data = &(m_data[name] = newEntry);
	}
	else
		data = &it.value();

	// Check if the interval list is full yet
	if (data->m_interval.size() == 10)
	{
		// If it is, take average
		Time average(0);
		for (Uint32 i = 0; i < data->m_interval.size(); ++i)
			average += data->m_interval[i];

		// Add the average to the list
		data->m_averages.push_back(average / 10.0);

		// Clear the interval list
		data->m_interval.clear();
	}

	// Add next data point
	data->m_interval.push_back(marker.getElapsedTime());
}


///////////////////////////////////////////////////////////
const ProfilerData& Profiler::getData(const std::string& func, const std::string& label)
{
	ProfilerData* data;

	// Try to find data, if can't find, create new entry
	std::string name = label + ':' + func;

	// Get access to data
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_data.find(name);
	if (it == m_data.end())
	{
		// Create an entry if profiler marker hasn't been entered yet
		ProfilerData newEntry;
		newEntry.m_label = label;
		newEntry.m_func = func;
		newEntry.m_numRuns = 0;

		data = &(m_data[name] = newEntry);
	}
	else
		data = &it.value();

	return *data;
}


}