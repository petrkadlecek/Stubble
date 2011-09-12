#ifndef STUBBLE_HAIR_TASK_PROCESSOR_HPP
#define STUBBLE_HAIR_TASK_PROCESSOR_HPP

#include <deque>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>
#include "../Common/CommonTypes.hpp"
#include "newmat.h"
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
	typedef std::deque< HairTask* > TaskAccumulator;
	typedef Vector3D< Real > Vec3;
	typedef NEWMAT::ColumnVector RealN;
	typedef NEWMAT::Matrix RealNxN;

	///----------------------------------------------------------------------------------------------------
	/// Gets the instance of the class
	///
	/// \return The class instance
	///----------------------------------------------------------------------------------------------------
	static inline HairTaskProcessor *getInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Destroys existing instance of the class. Must be called before control reaches the end of program.
	///----------------------------------------------------------------------------------------------------
	static inline void destroyInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Returns true if the thread is running, contains critical section
	///
	/// \return Value indicating worker thread activity
	///----------------------------------------------------------------------------------------------------
	static inline bool isRunning ();

	///----------------------------------------------------------------------------------------------------
	/// Synchronization barrier that waits until the thread finishes work, contains critical section
	///----------------------------------------------------------------------------------------------------
	static void waitFinishWorkerThread ();

	///----------------------------------------------------------------------------------------------------
	/// Enqueues new hair task into the accumulator queue and creates a worker thread if there's not one
	/// already active. Contains two critical sections.
	///
	/// \param aTask The new task to be added
	///----------------------------------------------------------------------------------------------------
	void enqueueTask (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Clears out the whole accumulator queue, contains critical section
	///----------------------------------------------------------------------------------------------------
	void purgeAccumulator ();

	///----------------------------------------------------------------------------------------------------
	/// Returns the size of the accumulator. Contains critical section
	///
	/// \return The size of the accumulator queue
	///----------------------------------------------------------------------------------------------------
	inline size_t getAccumulatorSize ();

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
	/// Tries to create new worker thread if there isn't already one active, contains critical section
	///----------------------------------------------------------------------------------------------------
	static void tryCreateWorkerThread ();

	///----------------------------------------------------------------------------------------------------
	/// Callback function that is called right after the thread finished. Sets the isRunning flag to
	/// false. Contains critical section.
	///
	/// \param aData Optional data supplied to the callback, see Maya API manual for more information
	///----------------------------------------------------------------------------------------------------
	static void workerFinishedCB (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// The actual code executed by the worker thread. Contains critical sections.
	///
	/// \param aData Optional data supplied to the worker thread, see Maya API manual for more information
	///----------------------------------------------------------------------------------------------------
	static MThreadRetVal asyncWorkerLoop (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// Method for getting task from the queue. If the queue is empty, the pointer returned is null.
	/// Contains critical section. Returns number of remaining objects in the queue.
	///
	/// \param aTask The task at the front of the queue
	/// \return Number of remaining tasks in the queue
	///----------------------------------------------------------------------------------------------------
	size_t getTask (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Calls doBrush method contained within the aTask object
	///
	/// \param aTask The task object
	///----------------------------------------------------------------------------------------------------
	void doBrush (HairShape::HairComponents::SelectedGuides &aSelectedGuides, const Vector3D< Real > &aDx, BrushMode *aBrushMode);

	///----------------------------------------------------------------------------------------------------
	/// Makes sure that hair retains their properties by minimizing an error functional
	///
	/// \param aTask The task object
	///----------------------------------------------------------------------------------------------------
	void enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides);

	static HairTaskProcessor *sInstance; ///< The class instance
	TaskAccumulator mTaskAccumulator; ///< The task queue
	MSpinLock mTaskAccumulatorLock; ///< Task queue spinlock
	static bool sIsRunning; ///< Flag for determining that the thread is active
	static MSpinLock sIsRunningLock; ///< isRunning spinlock

	static const Uint MAX_LOOP_ITERATIONS; ///< Maximum convergence loop iterations after which we consider solution converged
	static const Real CONVERGENCE_THRESHOLD; ///< Maximum allowed error at which we consider solution converged
	static const Uint RIGID_BODY_COUPL_CONSTRAINTS; ///< Number of rigidi body coupling constraints - 3 per rigid body (in our case just the hair root)
	static const Real INV_ROOT_SGMT_WEIGHT; ///< Inverted root segment weight (kg) - applied to the hair follicle coupling constraints
	static const Real INV_MID_SGMT_WEIGHT; ///< Inverted middle segment weight (kg) - applied to the rest of hair segment constraints
	static const Real DELTA_SCALE; ///< Scale factor of the delta matrix, in the article denoted as "h", square root of the dT element
};

inline HairTaskProcessor *HairTaskProcessor::getInstance ()
{
	if (0 == HairTaskProcessor::sInstance)
	{
		HairTaskProcessor::sInstance = new HairTaskProcessor();
	}

	return HairTaskProcessor::sInstance;
}

inline void HairTaskProcessor::destroyInstance ()
{
	if (HairTaskProcessor::sInstance)
	{
		HairTaskProcessor::sInstance->purgeAccumulator();
		HairTaskProcessor::waitFinishWorkerThread();

		delete HairTaskProcessor::sInstance;
	}

	HairTaskProcessor::sInstance = 0;
}

inline bool HairTaskProcessor::isRunning ()
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::sIsRunningLock.lock();
		bool result = HairTaskProcessor::sIsRunning;
	HairTaskProcessor::sIsRunningLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return result;
}

inline size_t HairTaskProcessor::getAccumulatorSize ()
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	mTaskAccumulatorLock.lock();
		size_t size = mTaskAccumulator.size();
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return size;
}

} // namespace Toolbox

} // namespace Stubble

#endif