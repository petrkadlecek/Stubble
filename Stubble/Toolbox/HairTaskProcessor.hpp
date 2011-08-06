#ifndef STUBBLE_HAIR_TASK_PROCESSOR_HPP
#define STUBBLE_HAIR_TASK_PROCESSOR_HPP

#include <deque>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>
#include "HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Singleton responsible for enqueing and asynchronously processing hair tasks issued by brush tools.
///----------------------------------------------------------------------------------------------------
class HairTaskProcessor
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Gets the instance of the class
	/// \return The class instance
	///----------------------------------------------------------------------------------------------------
	static inline HairTaskProcessor *getInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Destroys existing instance of the class. Must be called before control reaches the end of program.
	///----------------------------------------------------------------------------------------------------
	static inline void destroyInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Returns true if the thread is running, contains critical section
	/// \return Value indicating worker thread activity
	///----------------------------------------------------------------------------------------------------
	static inline bool isRunning ();

	///----------------------------------------------------------------------------------------------------
	/// Synchronisation barrier that waits until the thread finishes work
	///----------------------------------------------------------------------------------------------------
	static void waitFinishWorkerThread ();

	///----------------------------------------------------------------------------------------------------
	/// Enqueues new hair task into the accumulator queue
	/// \param aTask The new task to be added
	///----------------------------------------------------------------------------------------------------
	void enqueueTask (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Clears out the whole accumulator queue
	///----------------------------------------------------------------------------------------------------
	void purgeAccumulator ();

private:
	///----------------------------------------------------------------------------------------------------
	/// Default constructor
	///----------------------------------------------------------------------------------------------------
	HairTaskProcessor () {};

	///----------------------------------------------------------------------------------------------------
	/// Destructor
	///----------------------------------------------------------------------------------------------------
	~HairTaskProcessor () {};

	///----------------------------------------------------------------------------------------------------
	/// Copy constructor
	///----------------------------------------------------------------------------------------------------
	HairTaskProcessor (const HairTaskProcessor &aObj) {};

	///----------------------------------------------------------------------------------------------------
	/// Assignment operator
	///----------------------------------------------------------------------------------------------------
	HairTaskProcessor & operator = (const HairTaskProcessor &aObj) {};

	///----------------------------------------------------------------------------------------------------
	/// Tries to create new worker thread if there isn't already one active
	///----------------------------------------------------------------------------------------------------
	static void tryCreateWorkerThread ();

	///----------------------------------------------------------------------------------------------------
	/// Callback function that is called right after the thread finished
	/// \param aData Optional data supplied to the callback, see Maya API manual for more information
	///----------------------------------------------------------------------------------------------------
	static void workerFinishedCB (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// The actual code executed by the worker thread
	/// \param aData Optional data supplied to the worker thread, see Maya API manual for more information
	///----------------------------------------------------------------------------------------------------
	static void asyncWorkerLoop (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// Returns another task from the queue. If the queue is empty, returns dummy hair task object
	/// \return The next task object from the queue
	///----------------------------------------------------------------------------------------------------
	HairTask *getTask ();

	///----------------------------------------------------------------------------------------------------
	/// Calls doBrush method contained within the aTask object
	/// \param aTask The task object
	///----------------------------------------------------------------------------------------------------
	void doBrush (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Makes sure that hair retains their properties by minimizing an error functional
	/// \param aTask The task object
	///----------------------------------------------------------------------------------------------------
	void enforceConstraints (HairTask *aTask);

	static HairTaskProcessor *mInstance; ///< The class instance
	std::deque< HairTask > mTaskAccumulator; ///< The task queue
	MSpinLock mTaskAccumulatorLock; ///< Task queue spinlock
	static bool mIsRunning; ///< Flag for determining that the thread is active
	static MSpinLock mIsRunningLock; ///< isRunning spinlock

};

inline HairTaskProcessor *HairTaskProcessor::getInstance ()
{
	if (0 == HairTaskProcessor::mInstance)
	{
		HairTaskProcessor::mInstance = new HairTaskProcessor();
	}

	return HairTaskProcessor::mInstance;
}

inline void HairTaskProcessor::destroyInstance ()
{
	if (HairTaskProcessor::mInstance)
	{
		//TODO

		delete HairTaskProcessor::mInstance;
	}

	HairTaskProcessor::mInstance = 0;
}

inline bool HairTaskProcessor::isRunning ()
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::mIsRunningLock.lock();
		bool result = HairTaskProcessor::mIsRunning;
	HairTaskProcessor::mIsRunningLock.unlock();
	// ------------------------------------
	// End cricical section
	// ------------------------------------

	return result;
}

} // namespace Toolbox

} // namespace Stubble

#endif