#ifndef STUBBLE_TIMER_HPP
#define STUBBLE_TIMER_HPP

#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "maya\MGlobal.h"

namespace Stubble 
{
// Use to enable high performance counting throught MS counters.
#define MS_TIMER

// Declaration -----------------------------------------------------------

class Timer
{
public:
	Timer();		// Creates an empty timer

	void reset();	// Reset the timer
	void start();	// Start the timer
	void stop();	// Stop the timer and add the time to the total
					// time spent timeElapsed
	double getTimeFromStart();

	double getElapsedTime();
	double getLastElapsedTime();

	void mayaDisplayTimeFromStart();
	void mayaDisplayElapsedTime();
	void mayaDisplayLastElapsedTime();
#ifdef MS_TIMER
	__int64 getCount();
	static __int64 getGlobalTime();
#else
	long getCount();
	static long getGlobalTime();
#endif

  static long getGlobalTimeClock();

private:
#ifdef MS_TIMER
	__int64	mTimeElapsed;	// total time spent in all the call start/stop
	__int64	mLastTimeElapsed;// time spent in the last call start/stop
	__int64	mTimeStarted;	// system time from start
#else
	long	mTimeElapsed;	// total time spent in all the call start/stop
	long	mLastTimeElapsed;// time spent in the last call start/stop
	long	mTimeStarted;	// system time from start
#endif

#ifdef MS_TIMER
	__int64           mCounterFrequency;
	int               mCounterInitialized;
#endif

};

// Inline functions ------------------------------------------------------
inline Timer::Timer() 
{
	reset();
#ifdef MS_TIMER
	mCounterInitialized = QueryPerformanceFrequency( ( LARGE_INTEGER* ) &mCounterFrequency );
#endif
}

inline void Timer::reset() 
{
	mTimeElapsed = 0;
	mLastTimeElapsed = 0;
}

inline void Timer::start() 
{
#ifdef MS_TIMER
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &mTimeStarted );
#else
	mTimeStarted = clock();
#endif
}

inline void Timer::stop() 
{
#ifdef MS_TIMER
	__int64 t;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&t );
	mLastTimeElapsed = t - mTimeStarted;
#else
	mLastTimeElapsed = clock() - mTimeStarted;
#endif
	mTimeElapsed += mLastTimeElapsed;
}

inline double Timer::getTimeFromStart()
{
#ifdef MS_TIMER
	__int64 t;
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &t );
	return ( double )( t - mTimeStarted ) / ( double )mCounterFrequency;
#else
	return ( double )( clock() - mTimeStarted ) / ( double )CLOCKS_PER_SEC; 
#endif
}

inline double Timer::getLastElapsedTime() 
{
#ifdef MS_TIMER
	return ( double )mLastTimeElapsed / ( double )mCounterFrequency; 
#else
	return ( double )mLastTimeElapsed / ( double )CLOCKS_PER_SEC; 
#endif
}

inline double Timer::getElapsedTime() {
#ifdef MS_TIMER
	return ( double )mTimeElapsed / ( double )mCounterFrequency; 
#else
	return ( double )mTimeElapsed / ( double )CLOCKS_PER_SEC; 
#endif
}

inline long Timer::getGlobalTimeClock() 
{
	long t = clock();
	return t;
}

inline void Timer::mayaDisplayElapsedTime()
{
	MString hodnota;
	hodnota.set( getElapsedTime() );
	MGlobal::displayInfo( "ElapsedTime: " + hodnota + " s" );
}

inline void Timer::mayaDisplayLastElapsedTime()
{
	MString hodnota;
	hodnota.set( getLastElapsedTime() );
	MGlobal::displayInfo( "LastElapsedTime: " + hodnota + " s" );
}

inline void Timer::mayaDisplayTimeFromStart()
{
	MString hodnota;
	hodnota.set( getTimeFromStart() );
	MGlobal::displayInfo( "TimeFromStart: " + hodnota + " s" );
}

} // namespace Stubble

#endif // STUBBLE_TIMER_HPP
