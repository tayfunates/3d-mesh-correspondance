#include "core/Timer.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#include <sys/time.h>
#include <time.h>
#endif

#ifdef __MACH__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif


namespace TACore
{

	Timer::Timer()
	{
		reset();
	}

	void Timer::reset()
	{
		m_start = now();
	}

	double Timer::seconds() const
	{
		return now() - m_start;
	}

	double Timer::now()
	{
#ifdef _MSC_VER
		LARGE_INTEGER freq;
		LARGE_INTEGER t;
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&t);
		return double(t.QuadPart) / double(freq.QuadPart);
#elif defined( __MACH__ )
		static mach_timebase_info_data_t sTimebaseInfo;
		uint64_t t = mach_absolute_time();
		if (sTimebaseInfo.denom == 0)
		{
			(void)mach_timebase_info(&sTimebaseInfo);
		}
		return (t * sTimebaseInfo.numer / sTimebaseInfo.denom) * 0.000000001;
#else
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		return double(ts.tv_sec) + ts.tv_nsec * 0.000000001;
#endif
	}



} // namespace TACore
