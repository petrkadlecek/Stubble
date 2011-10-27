#ifndef STUBBLE_HAIR_TASK_PROCESSOR_HPP
#define STUBBLE_HAIR_TASK_PROCESSOR_HPP

#include <deque>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>
#include <maya/MMutexLock.h>
#include "Common/CommonTypes.hpp"
#include "HairShape/HairComponents/Segments.hpp"
#include "newmat.h"
#include "HairTask.hpp"

//#define STUBBLE_ORIGINAL_HAIRSTYLING 1

namespace Stubble
{

namespace Toolbox
{

typedef NEWMAT::ColumnVector RealN;
typedef NEWMAT::Matrix RealNxN;
	
///----------------------------------------------------------------------------------------------------
/// Singleton responsible for enqueing and asynchronously processing hair tasks issued by brush tools.
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
	/// Makes sure that hair retains their properties by minimizing an error functional. It takes into
	/// account inextensibility constraints and guide-mesh interpenetration constraints. Implementation
	/// of the fast manifold projection method - see appropriate paper.
	///
	/// \param aSelectedGuides	Guides to be processed
	///----------------------------------------------------------------------------------------------------
	static void enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides);
	
	///----------------------------------------------------------------------------------------------------
	/// A lightweight version of enforceConstraints that takes into account only  inextensibility and
	/// operates only on one guide's segments w/o any additional information.
	///
	/// \param aVertices		Guide vertices to be processed
	/// \param aSegmentLength	Length of a single segment
	///----------------------------------------------------------------------------------------------------
	static void enforceConstraints(HairShape::HairComponents::Segments &aVertices, Real aSegmentLength);

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
	size_t getTask (HairTask *&aTask);

	///----------------------------------------------------------------------------------------------------
	/// Calls doBrush method contained within the aTask object
	///
	/// \param aTask The task object
	///----------------------------------------------------------------------------------------------------
	void doBrush (HairShape::HairComponents::SelectedGuides &aSelectedGuides, const Vector3D< Real > &aDx, BrushMode *aBrushMode);

	///----------------------------------------------------------------------------------------------------
	/// Detects collisions for the manipulated segments and count also penetration.
	///
	/// \param aSelectedGuides Selection of guides for collision calculation
	///----------------------------------------------------------------------------------------------------
	void detectCollisions( HairShape::HairComponents::SelectedGuides &aSelectedGuides );

	///----------------------------------------------------------------------------------------------------
	/// Fills the beginning of the constraint vector with inextensibility constraints. Doesn't do any bound
	/// checking.
	///
	/// \param aC				Out - the constraint vector
	/// \param aHairVertices	Hair vertices to calculate constraints from
	/// \param aSgmtLengthSq	Desired length of the segment squared
	///----------------------------------------------------------------------------------------------------
	inline static void computeInextensibilityConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices, const Real aSgmtLengthSq);

	///----------------------------------------------------------------------------------------------------
	/// Fills the fixed part of the nabla C matrix with inextensibility constraint derivatives. Doesn't
	/// do any bound checking. Calculates also the nabla C transpose (delta).
	///
	/// \param aNC				Out - the nabla C matrix
	/// \param aDelta			Out - the nabla C matrix transpose
	/// \param aHairVertices	Hair vertices to calculate derivatives from
	///----------------------------------------------------------------------------------------------------
	inline static void computeInextensibilityGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices);

	///----------------------------------------------------------------------------------------------------
	/// Fills the flexible part of the constraint vector with interpenetration constraints. Doesn't do any
	/// bound checking.
	///
	/// \param aC				Out - the constraint vector
	/// \param aHairVertices	Hair vertices to calculate constraints from
	/// \param aVerticesInfo	Additional info for hair vertices
	/// \param aOffset			Offset just after the inextensibility constraints part
	///----------------------------------------------------------------------------------------------------
	inline static void computeInterpenetrationConstraints(RealN &aC, HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aOffset);

	///----------------------------------------------------------------------------------------------------
	/// Fills the flexible part of the nabla C matrix with interpenetration constraint derivatives. Doesn't
	/// do any bound checking. Calculates also the nabla C transpose (delta).
	///
	/// \param aNC				Out - the nabla C matrix
	/// \param aDelta			Out - the nabla C matrix transpose
	/// \param aHairVertices	Hair vertices to calculate derivatives from
	/// \param aVerticesInfo	Additional info for hair vertices
	/// \param aConstrOffset	Offset just after the inextensibility constraints part - row space
	/// \param aDerivOffset		Offset just after the inextensibility constraints part - column space
	///----------------------------------------------------------------------------------------------------
	inline static void computeInterpenetrationGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aConstrOffset, const Uint aDerivOffset);

	static HairTaskProcessor *sInstance; ///< The class instance
	TaskAccumulator mTaskAccumulator; ///< The task queue
	MSpinLock mTaskAccumulatorLock; ///< Task queue spinlock
	static bool sIsRunning; ///< Flag for determining that the thread is active
	static MSpinLock sIsRunningLock; ///< isRunning spinlock

	static const Uint MAX_LOOP_ITERATIONS; ///< Maximum convergence loop iterations after which we consider solution converged
	static const Real CONVERGENCE_THRESHOLD; ///< Maximum allowed error at which we consider solution converged
	static const Real EPSILON; ///< Epsilon for float equals operations
#ifdef STUBBLE_ORIGINAL_HAIRSTYLING
	static const Uint RIGID_BODY_COUPL_CONSTRAINTS; ///< Number of rigidi body coupling constraints - 3 per rigid body (in our case just the hair root)
	static const Real INV_ROOT_SGMT_WEIGHT; ///< Inverted root segment weight (kg) - applied to the hair follicle coupling constraints
	static const Real INV_MID_SGMT_WEIGHT; ///< Inverted middle segment weight (kg) - applied to the rest of hair segment constraints
	static const Real DELTA_SCALE; ///< Scale factor of the delta matrix, in the article denoted as "h", square root of the dT element
#endif
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
		aC[ aOffset + j ] = Vec3::dotProduct(e, e);
		++j;
	}
}

void HairTaskProcessor::computeInterpenetrationGradient(RealNxN &aNC, RealNxN &aDelta, HairShape::HairComponents::Segments &aHairVertices,
		HairShape::HairComponents::SegmentsAdditionalInfo &aVerticesInfo, const Uint aConstrOffset, const Uint aDerivOffset)
{
	const Uint VERTEX_COUNT = (Uint)aHairVertices.size();
	assert( VERTEX_COUNT == (Uint)aVerticesInfo.size() );
	Uint j = 0; // NC and delta matrices index
	Vec3 e;
	for (Uint i = 1; i < VERTEX_COUNT; ++i) // We don't check the root for obvious reasons
	{
		if (!aVerticesInfo[ i ].mIsColliding)
		{
			continue;
		}
		e = (aVerticesInfo[ i ].mClosestPointOnMesh - aHairVertices[ i ]) * 2.0;
		aNC[ aConstrOffset + j ][ aDerivOffset + 3*j ] = -e.x;
		aDelta[ aDerivOffset + 3*j ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ aDerivOffset + 3*j ];
		aNC[ aConstrOffset + j ][ aDerivOffset + 3*j + 1 ] = -e.y;
		aDelta[ aDerivOffset + 3*j + 1 ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ aDerivOffset + 3*j + 1 ];
		aNC[ aConstrOffset + j ][ aDerivOffset + 3*j + 2 ] = -e.z;
		aDelta[ aDerivOffset + 3*j + 2 ][ aConstrOffset + j ] = aNC[ aConstrOffset + j ][ aDerivOffset + 3*j + 2 ];
		++j;
	}
}

} // namespace Toolbox

} // namespace Stubble

#endif