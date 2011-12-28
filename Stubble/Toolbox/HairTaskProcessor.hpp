#ifndef STUBBLE_HAIR_TASK_PROCESSOR_HPP
#define STUBBLE_HAIR_TASK_PROCESSOR_HPP

#include <deque>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>
#include "Common/CommonTypes.hpp"
#include "Common/StubbleTimer.hpp"
#include "HairShape/HairComponents/Segments.hpp"
#include "newmat.h"
#include "HairTask.hpp"
#include "Common/StubbleTimer.hpp"

namespace Stubble
{

namespace Toolbox
{
// Arbitrary sized vectors and matrices - Intel MKL could probably yield better performance
typedef NEWMAT::ColumnVector RealN;
typedef NEWMAT::Matrix RealNxN;
	
///----------------------------------------------------------------------------------------------------
/// Singleton responsible for queuing and asynchronously processing hair tasks issued by brush tools.
///----------------------------------------------------------------------------------------------------
class HairTaskProcessor
{
public:

	typedef std::deque< HairTask* > TaskAccumulator;
	typedef Vector3D< Real > Vec3;
	
	///----------------------------------------------------------------------------------------------------
	/// Gets the instance of the class
	///
	/// \return The class instance
	///----------------------------------------------------------------------------------------------------
	inline static HairTaskProcessor *getInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Destroys existing instance of the class. Must be called before control reaches the end of program.
	/// Purges the accumulator queue and waits for the worker thread to join.
	///----------------------------------------------------------------------------------------------------
	inline static void destroyInstance ();

	///----------------------------------------------------------------------------------------------------
	/// Returns true if the thread is running, contains critical section
	///
	/// \return Value indicating worker thread activity
	///----------------------------------------------------------------------------------------------------
	inline static bool isRunning ();

	///----------------------------------------------------------------------------------------------------
	/// Synchronization barrier that waits until the thread finishes work, contains critical section
	///----------------------------------------------------------------------------------------------------
	static void waitFinishWorkerThread ();

	///----------------------------------------------------------------------------------------------------
	/// Forces the worker thread to stop, i.e. purges the accumulator, skips waiting for possible new tasks
	/// and waits until the loop ends. Calls purgeAccumulator and waitFinishWorkerThread
	///----------------------------------------------------------------------------------------------------
	static void stopWorkerThread();

	///----------------------------------------------------------------------------------------------------
	/// Makes sure that hair retains their properties by minimizing an error functional. It takes into
	/// account inextensibility constraints and guide-mesh interpenetration constraints. Implements the
	/// fast manifold projection method. Iteratively calculates first step of Newtonian minimization
	/// of hair energy functional and applies corrections to the hair until convergence criteria are met.
	/// Doesn't calculate twisting and other dynamic stuff as mentioned in the paper. For further details see:
	/// Kmoch, P., Bonnani, U., Magnetat-Thalmann, N. - Hair simulation model for real-time environments
	///
	/// Usually called from the worker thread, because of potentially slow execution.
	///
	/// \param aSelectedGuides Guides to be processed
	///----------------------------------------------------------------------------------------------------
	static void enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides);
	
	///----------------------------------------------------------------------------------------------------
	/// A lightweight version of enforceConstraints that takes into account only inextensibility and
	/// operates only on one guide segments w/o any additional information.
	/// 
	/// Usually called after hair cutting from the main thread. Mind the potentially slow execution.
	///
	/// \param aVertices Guide vertices to be processed
	/// \param aSegmentLength Length of a single segment
	///----------------------------------------------------------------------------------------------------
	static void enforceConstraints(HairShape::HairComponents::Segments &aVertices, Real aSegmentLength);

	///----------------------------------------------------------------------------------------------------
	/// Enqueues new hair task into the accumulator queue and creates a worker thread if there's not one
	/// already active. Contains two critical sections. Called from the main thread.
	///
	/// \param aTask The new task to be added
	///----------------------------------------------------------------------------------------------------
	void enqueueTask (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Clears out the whole accumulator queue, contains critical section. Usually called from the main
	/// thread.
	///----------------------------------------------------------------------------------------------------
	void purgeAccumulator ();

	///----------------------------------------------------------------------------------------------------
	/// Returns the size of the accumulator. Contains critical section. Called from the worker thread.
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
	/// \param aData Optional data supplied to the callback, see Maya API reference for more information
	///----------------------------------------------------------------------------------------------------
	static void workerFinishedCB (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// The actual code executed by the worker thread. Contains critical sections. Until the queue is empty
	/// executes hair brushing transformation, collision detection and constraint enforcement. Thanks to this
	/// decoupling motion integration is easily and quickly computed and hair constraints are enforces as
	/// post-integration step. See \link enforceConstraints \endlink for more details.
	///
	/// \param aData Optional data supplied to the worker thread, see Maya API reference for more information
	///
	/// \return Thread return value, see Maya API reference for more information
	///----------------------------------------------------------------------------------------------------
	static MThreadRetVal asyncWorkerLoop (void *aData);

	///----------------------------------------------------------------------------------------------------
	/// Method for getting task from the queue. If the queue is empty, the pointer returned is not altered.
	/// Contains critical section. Returns number of remaining objects in the queue.
	///
	/// \param[out] aTask The task at the front of the queue if any
	///
	/// \return Number of remaining tasks in the queue
	///----------------------------------------------------------------------------------------------------
	size_t getTask (HairTask *&aTask);

	///----------------------------------------------------------------------------------------------------
	/// Method for accumulating tasks from the queue. Assumes the queue is locked and non-empty. Doesn't
	/// perform any bound checking.
	///
	/// \return Accumulated task
	///----------------------------------------------------------------------------------------------------
	HairTask *accumulate ();

	///----------------------------------------------------------------------------------------------------
	/// Called from the worker thread loop to process a single hair task, i.e. apply brushing transformation,
	/// detect collisions (optionally), and enforce constraints. Expects non-null pointer.
	///
	/// \param aTask The task to be processed
	///----------------------------------------------------------------------------------------------------
	inline static void processTask (HairTask *aTask);

	///----------------------------------------------------------------------------------------------------
	/// Detects collisions for manipulated segments, counts number of collisions and finds out closest
	/// points on mesh for all colliding vertices. These information are written to auxiliary structure
	/// in the OneSelectedGuide class.
	///
	/// \param aSelectedGuides Selection of guides for collision calculation
	///----------------------------------------------------------------------------------------------------
	static void detectCollisions (HairShape::HairComponents::SelectedGuides &aSelectedGuides);

	///----------------------------------------------------------------------------------------------------
	/// Checks the collision information after each enforceConstraints minimization step and reduces the
	/// collision set if any of the vertices no longer penetrates the mesh. It makes the assumption that
	/// no new collisions may occur - possible new collisions will be treated in the next worker loop
	/// iteration. We don't even have information for them and once we discard a point from the collision
	/// set, we don't have means to tell whether it was previously colliding or not.
	///
	/// \param aHairVertices Hair vertices positions
	/// \param[in,out] aVerticesInfo Additional info for hair vertices containing collision information
	///
	/// \return Number of colliding vertices
	///----------------------------------------------------------------------------------------------------
	inline static Uint updateCollisionInfo (const HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo);

	///----------------------------------------------------------------------------------------------------
	/// Fills the beginning of the constraint vector with inextensibility constraints. Doesn't do any bound
	/// checking.
	///
	/// \param[in,out] aC The constraint vector
	/// \param aHairVertices Hair vertices to calculate constraints from
	/// \param aSgmtLengthSq Desired length of the segment squared
	///----------------------------------------------------------------------------------------------------
	inline static void computeInextensibilityConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices, const Real aSgmtLengthSq);

	///----------------------------------------------------------------------------------------------------
	/// Fills the fixed part of the nabla C matrix with inextensibility constraint derivatives. Doesn't
	/// do any bound checking. Calculates also the nabla C transpose (delta).
	///
	/// \param[in,out] aNC The nabla C matrix
	/// \param[in,out] aDelta The nabla C matrix transpose
	/// \param aHairVertices Hair vertices to calculate derivatives from
	///----------------------------------------------------------------------------------------------------
	inline static void computeInextensibilityGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices);

	///----------------------------------------------------------------------------------------------------
	/// Fills the flexible part of the constraint vector with interpenetration constraints. Doesn't do any
	/// bound checking.
	///
	/// \param[in,out] aC The constraint vector
	/// \param aHairVertices Hair vertices to calculate constraints from
	/// \param aVerticesInfo Additional info for hair vertices
	/// \param aOffset Offset just after the inextensibility constraints part
	///----------------------------------------------------------------------------------------------------
	inline static void computeInterpenetrationConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aOffset);

	///----------------------------------------------------------------------------------------------------
	/// Fills the flexible part of the nabla C matrix with interpenetration constraint derivatives. Doesn't
	/// do any bound checking. Calculates also the nabla C transpose (delta).
	///
	/// \param[in,out] aNC The nabla C matrix
	/// \param[in,out] aDelta The nabla C matrix transpose
	/// \param aHairVertices Hair vertices to calculate derivatives from
	/// \param aVerticesInfo Additional info for hair vertices
	/// \param aConstrOffset Offset just after the inextensibility constraints part - row space
	///----------------------------------------------------------------------------------------------------
	inline static void computeInterpenetrationGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aConstrOffset);

	///----------------------------------------------------------------------------------------------------
	/// Helper method for rescaling guide hair vertices by an arbitrary scale factor.
	///
	/// \param[in,out] aVertices Hair vertices to be scaled
	/// \param aScaleFactor Scale factor to be applied
	///----------------------------------------------------------------------------------------------------
	inline static void rescaleGuideHair(HairShape::HairComponents::Segments &aVertices, Real aScaleFactor);
	
	///----------------------------------------------------------------------------------------------------
	/// Helper method for rescaling closest points on mesh for all colliding hair vertices by an arbitrary
	/// scale factor.
	///
	/// \param[in,out] aVerticesInfo Container containing closest points on mesh
	/// \param aScaleFactor Scale factor to be applied
	///----------------------------------------------------------------------------------------------------
	inline static void rescaleClosestPoints(HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, Real aScaleFactor);

	TaskAccumulator mTaskAccumulator; ///< The task queue
	MSpinLock mTaskAccumulatorLock; ///< Task queue spinlock
	Timer mTimer; ///< Timeout timer for thread suicide

	static HairTaskProcessor *sInstance; ///< The class instance
	static bool sIsRunning; ///< Flag for determining that the thread is active
	static MSpinLock sIsRunningLock; ///< isRunning spinlock
	static volatile bool sRun; ///< Flag for determining that the thread should still run

	static const Real MAX_IDLE_TIME; ///< Maximum time of the idle worker loop iterations
	static const Uint MAX_LOOP_ITERATIONS; ///< Maximum convergence loop iterations after which we consider solution converged
	static const size_t MAX_TASK_QUEUE_SIZE; ///< Maximum size of the task queue to prevent overloading
	static const Real CONVERGENCE_THRESHOLD; ///< Maximum allowed error at which we consider solution converged
	static const Real MAX_TASK_DX; ///< Maximal allowed hair change to prevent numerical stiffness
	static const Real MAX_TASK_DX_SQ; ///< Maximal allowed hair change (squared) to prevent numerical stiffness
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

inline void HairTaskProcessor::processTask(HairTask *aTask)
{
	assert( 0 != aTask );

	aTask->mBrushMode->doBrush(aTask);
	if ( aTask->mBrushMode->isCollisionDetectionEnabled() )
	{
		HairTaskProcessor::detectCollisions( *(aTask->mAffectedGuides) );
	}
	HairTaskProcessor::enforceConstraints( *(aTask->mAffectedGuides) );
}

inline Uint HairTaskProcessor::updateCollisionInfo (const HairShape::HairComponents::Segments &aHairVertices,
	HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	assert( VERTEX_COUNT == (Uint)aVerticesInfo.size() );

	Uint collisionsCount = 0;
	Vec3 v, dir; // Actual direction towards closest point and initial direction towards the surface
	for (Uint i = 1; i < VERTEX_COUNT; ++i) // For all vertices except the root
	{
		if (!aVerticesInfo[ i ].mIsColliding)
		{
			continue;
		}
		v = aVerticesInfo[ i ].mClosestPointOnMesh - aHairVertices[ i ];
		dir = aVerticesInfo[ i ].mSurfaceDirection;
		if ( Vec3::dotProduct(v, dir) > 0.0 )
		{
			aVerticesInfo[ i ].mIsColliding = false;
		}
		else
		{
			collisionsCount++;
		}
	}

	return collisionsCount;
}

inline void HairTaskProcessor::computeInextensibilityConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices, const Real aSgmtLengthSq)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	Vec3 e;
	for (Uint i = 0; i < VERTEX_COUNT - 1; ++i) // For all line segments
	{
		e = aHairVertices[ i + 1 ] - aHairVertices[ i ];
		aC[ i ] = Vec3::dotProduct(e, e) - aSgmtLengthSq;
	}
}

inline void HairTaskProcessor::computeInextensibilityGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	Vec3 e = (aHairVertices[ 1 ] - aHairVertices[ 0 ]) * 2.0; // First segment treated separately - only derivatives for the second vertex are calculated
	aNC[ 0 ][ 0 ] = e.x;
	aDelta[ 0 ][ 0 ] = aNC[ 0 ][ 0 ];
	aNC[ 0 ][ 1 ] = e.y;
	aDelta[ 1 ][ 0 ] = aNC[ 0 ][ 1 ];
	aNC[ 0 ][ 2 ] = e.z;
	aDelta[ 2 ][ 0 ] = aNC[ 0 ][ 2 ];

	for (Uint i = 1; i < VERTEX_COUNT - 1; ++i) // All other segments calculated normally
	{
		e = (aHairVertices[ i + 1 ] - aHairVertices[ i ]) * 2.0;
		aNC[ i ][ 3*(i - 1) ] = -e.x;
		aDelta[ 3*(i - 1) ][ i ] = aNC[ i ][ 3*(i - 1) ];
		aNC[ i ][ 3*i ] = e.x;
		aDelta[ 3*i ][ i ] = aNC[ i ][ 3*i ];
		aNC[ i ][ 3*(i - 1) + 1 ] = -e.y;
		aDelta[ 3*(i - 1) + 1 ][ i ] = aNC[ i ][ 3*(i - 1) + 1 ];
		aNC[ i ][ 3*i + 1 ] = e.y;
		aDelta[ 3*i + 1 ][ i ] = aNC[ i ][ 3*i + 1 ];
		aNC[ i ][ 3*(i - 1) + 2 ] = -e.z;
		aDelta[ 3*(i - 1) + 2 ][ i ] = aNC[ i ][ 3*(i - 1) + 2 ];
		aNC[ i ][ 3*i + 2 ] = e.z;
		aDelta[ 3*i + 2 ][ i ] = aNC[ i ][ 3*i + 2 ];
	}
}

inline void HairTaskProcessor::computeInterpenetrationConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices,
	HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aOffset)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	assert( VERTEX_COUNT == (Uint)aVerticesInfo.size() );
	Uint j = 0; // Constraint vector index
	Vec3 e;
	for (Uint i = 1; i < VERTEX_COUNT; ++i) // We don't check the root for obvious reasons
	{
		if (!aVerticesInfo[ i ].mIsColliding)
		{
			continue;
		}
		e = aVerticesInfo[ i ].mClosestPointOnMesh - aHairVertices[ i ];
		aC[ aOffset + j ] = Vec3::dotProduct(e, e) + EPSILON;
		++j;
	}
}

inline void HairTaskProcessor::computeInterpenetrationGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices,
	HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aConstrOffset)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	assert( VERTEX_COUNT == (Uint)aVerticesInfo.size() );
	Uint j = 0; // NC and delta matrices row/column index
	Vec3 e;
	// For all vertices except the first
	for (Uint i = 1; i < VERTEX_COUNT; ++i)
	{
		if (!aVerticesInfo[ i ].mIsColliding)
		{
			continue;
		}
		e = (aVerticesInfo[ i ].mClosestPointOnMesh - aHairVertices[ i ]) * 2.0;
		aNC[ aConstrOffset + j ][ 3*(i - 1) ] = -e.x + EPSILON;
		aDelta[ 3*(i - 1) ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ 3*(i - 1) ];
		aNC[ aConstrOffset + j ][ 3*(i - 1) + 1 ] = -e.y + EPSILON;
		aDelta[ 3*(i - 1) + 1 ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ 3*(i - 1) + 1 ];
		aNC[ aConstrOffset + j ][ 3*(i - 1) + 2 ] = -e.z + EPSILON;
		aDelta[ 3*(i - 1) + 2 ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ 3*(i - 1) + 2 ];
		++j;
	}
}

inline void HairTaskProcessor::rescaleGuideHair(HairShape::HairComponents::Segments &aVertices, Real aScaleFactor)
{
	HairShape::HairComponents::Segments::iterator it;
	for (it = aVertices.begin(); it != aVertices.end(); ++it)
	{
		*it *= aScaleFactor;
	}
}

inline void HairTaskProcessor::rescaleClosestPoints(HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, Real aScaleFactor)
{
	HairShape::HairComponents::SegmentsAdditionalInfo::iterator it;
	for (it = aVerticesInfo.begin(); it != aVerticesInfo.end(); ++it)
	{
		it->mClosestPointOnMesh *= aScaleFactor;
	}
}

} // namespace Toolbox

} // namespace Stubble

#endif
