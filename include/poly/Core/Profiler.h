#ifndef POLY_PROFILER_H
#define POLY_PROFILER_H

#include <poly/Core/Clock.h>
#include <poly/Core/DataTypes.h>
#include <poly/Core/Macros.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace poly
{

struct ProfilerData
{
	ProfilerData();

	Time mean() const;
	Time stdDev() const;

	std::string m_label;
	std::string m_func;
	std::vector<Time> m_interval;
	std::vector<Time> m_averages;
	Uint32 m_numRuns;
};

class ProfilerMarker
{
public:
	ProfilerMarker();
	ProfilerMarker(const std::string& label, const std::string& func);
	~ProfilerMarker();

	void start();
	void stop();

	const Time& getElapsedTime() const;
	const std::string& getLabel() const;
	const std::string& getFunc() const;

private:
	Clock m_clock;
	Time m_elapsedTime;
	std::string m_label;
	std::string m_func;
};

class Profiler
{
public:
	static void addMarker(const ProfilerMarker& marker);

	static const ProfilerData& getData(const std::string& func, const std::string& label = "");

private:
	static std::unordered_map<std::string, ProfilerData> m_data;
};

#ifdef ENABLE_PROFILING

#define START_PROFILING_FUNC \
poly::ProfilerMarker Profiler_("", __FUNCTION__); \
Profiler_.start();

#define START_PROFILING(label) \
poly::ProfilerMarker CONCAT(Profiler_, label)(STR(label), __FUNCTION__); \
CONCAT(Profiler_, label).start();

#define STOP_PROFILING(label) CONCAT(Profiler_, label).stop();

#else
#define START_PROFILING_FUNC
#define START_PROFILING(x)
#define STOP_PROFILING(x)
#endif

}

#endif