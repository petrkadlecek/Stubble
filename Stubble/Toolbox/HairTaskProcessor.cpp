#include "HairTaskProcessor.hpp"
#include "HairShape\UserInterface\HairShape.hpp"
#include <maya\MMeshIntersector.h>
#include <maya\MFloatPointArray.h>

#include <algorithm>
#include <iostream>

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members and constants:
// ----------------------------------------------------------------------------

HairTaskProcessor *HairTaskProcessor::sInstance = 0;
bool HairTaskProcessor::sIsRunning = false;
MSpinLock HairTaskProcessor::sIsRunningLock;
volatile bool HairTaskProcessor::sRun = true;

const Real HairTaskProcessor::MAX_IDLE_TIME = 0.5; // seconds
const Uint HairTaskProcessor::MAX_LOOP_ITERATIONS = 12;
const size_t HairTaskProcessor::MAX_TASK_QUEUE_SIZE = 50;
const Real HairTaskProcessor::CONVERGENCE_THRESHOLD = 1e-8;
const Real HairTaskProcessor::DELTA = 1e-4;
const Real HairTaskProcessor::MAX_TASK_DX = 0.5;
const Real HairTaskProcessor::MAX_TASK_DX_SQ = HairTaskProcessor::MAX_TASK_DX * HairTaskProcessor::MAX_TASK_DX;

// ----------------------------------------------------------------------------
// Methods:
// ----------------------------------------------------------------------------

void HairTaskProcessor::waitFinishWorkerThread ()
{
	while ( HairTaskProcessor::isRunning() ) // Contains critical section
	{
#ifdef _WIN32
		Sleep(0);
#else
		sleep(0);
#endif
	}
	//std::cout << std::endl << std::flush;
}

void HairTaskProcessor::stopWorkerThread ()
{
	HairTaskProcessor *instance = HairTaskProcessor::getInstance();
	if (0 == instance)
	{
		return;
	}

	instance->purgeAccumulator();
	HairTaskProcessor::sRun = false;
	waitFinishWorkerThread();
}

void HairTaskProcessor::enqueueTask (HairTask *aTask)
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	mTaskAccumulatorLock.lock();
		// If the queue is overloaded we're falling behind schedule anyway and
		// when the user releases mouse button any unprocessed tasks are removed
		if ( mTaskAccumulator.size() < HairTaskProcessor::MAX_TASK_QUEUE_SIZE )
		{
			mTaskAccumulator.push_back(aTask); //TODO: exception handling?
		}
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	HairTaskProcessor::tryCreateWorkerThread(); // Contains critical section
}

void HairTaskProcessor::purgeAccumulator ()
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	mTaskAccumulatorLock.lock();
		TaskAccumulator::iterator it;
		for (it = mTaskAccumulator.begin(); it != mTaskAccumulator.end(); ++it)
		{
			delete *it;
		}
		mTaskAccumulator.clear();
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------
}

void HairTaskProcessor::tryCreateWorkerThread ()
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::sIsRunningLock.lock();
		if (!HairTaskProcessor::sIsRunning)
		{
			MStatus status = MThreadAsync::init();
			if ( MStatus::kSuccess != status )
			{
				status.perror( "HairTaskProcessor: Failed to acquire thread resources" );
				//TODO: exception?
				goto END;
			}

			//std::cout << "(" << std::flush;
			HairTaskProcessor::sIsRunning = true;
			status = MThreadAsync::createTask(HairTaskProcessor::asyncWorkerLoop, 0, HairTaskProcessor::workerFinishedCB, 0);
			if ( MStatus::kSuccess != status )
			{
				status.perror( "HairTaskProcessor: Failed to run the worker thread" );
				//TODO: exception?
			}
		}
END:
	HairTaskProcessor::sIsRunningLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------
}

void HairTaskProcessor::workerFinishedCB (void *aData)
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::sIsRunningLock.lock();
		//std::cout << ")" << std::flush;
		HairTaskProcessor::sIsRunning = false;
		MThreadAsync::release();
	HairTaskProcessor::sIsRunningLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------
}

MThreadRetVal HairTaskProcessor::asyncWorkerLoop (void *aData)
{
	HairTaskProcessor::sRun = true;

	HairTaskProcessor *hairTaskProcessor = HairTaskProcessor::getInstance();
	HairTask *task = 0;
	size_t accumulatorSize = 0;
	Real idleTime = 0.0;
	
	hairTaskProcessor->mTimer.reset();
	while (HairTaskProcessor::sRun)
	{
		accumulatorSize = hairTaskProcessor->getTask(task);
		if (0 != task)
		{
			idleTime = 0.0;
			HairTaskProcessor::processTask(task);
			delete task;
			task = 0;
		}

		if (accumulatorSize == 0) // Loop idling
		{
			hairTaskProcessor->mTimer.stop();
			idleTime = hairTaskProcessor->mTimer.getElapsedTime();
			hairTaskProcessor->mTimer.start();

			if (idleTime > HairTaskProcessor::MAX_IDLE_TIME)
			{
				HairTaskProcessor::sRun = false;
			}
		}
	}
	hairTaskProcessor->mTimer.stop();

	return 0;
}

size_t HairTaskProcessor::getTask (HairTask *&aTask)
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	mTaskAccumulatorLock.lock();
		size_t queueSize = mTaskAccumulator.size();
		if ( queueSize > 0 )
		{
			aTask = accumulate();
			queueSize = mTaskAccumulator.size();
		}
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return queueSize;
}

HairTask *HairTaskProcessor::accumulate ()
{
	HairTask *accumTask = mTaskAccumulator.front();
	mTaskAccumulator.pop_front();

	Vec3 dir = accumTask->mDx;

	while ( !mTaskAccumulator.empty() )
	{
		HairTask *task = mTaskAccumulator.front();

		if ( Vec3::dotProduct(dir, task->mDx) > 0.0)
		{
			accumTask->mDx += task->mDx;
			mTaskAccumulator.pop_front();
			delete task;
		}
		else
		{
			break;
		}
	}
	
	if (accumTask->mDx.sizePwr2() > MAX_TASK_DX_SQ)
	{
		Vec3 v = Vec3::normalize(accumTask->mDx);
		v *= MAX_TASK_DX;
		accumTask->mDx = v;
	}

	return accumTask;
}

void HairTaskProcessor::detectCollisions( HairShape::HairComponents::SelectedGuides &aSelectedGuides )
{
	MMeshIntersector *accelerator =  HairShape::HairShape::getActiveObject()->getCurrentMesh().getMeshIntersector();
	MFnMesh *currentMesh = HairShape::HairShape::getActiveObject()->getCurrentMesh().getMayaMesh();
	MMatrix inclusiveMatrix = HairShape::HairShape::getActiveObject()->getCurrentInclusiveMatrixInverse();	
	MMeshIsectAccelParams accelParam = currentMesh->autoUniformGridParams();

	for( HairShape::HairComponents::SelectedGuides::iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
	{
		HairShape::HairComponents::SelectedGuide *guide = *it;
		guide->mCollisionsCount = 0;

		if (guide->mGuideSegments.mSegmentLength <= EPSILON)
		{
			continue;
		}

		assert( guide->mGuideSegments.mSegments.size() == guide->mSegmentsAdditionalInfo.size() );

		Matrix< Real > &worldMatrix = guide->mPosition.mWorldTransformMatrix;
		Matrix< Real > &localMatrix = guide->mPosition.mLocalTransformMatrix;

		const Vec3 normal = guide->mNormal;
		Vec3 firstPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(1), worldMatrix );
		Vec3 rootPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(0), worldMatrix ) + normal * 0.01;

		MFloatPoint startP(rootPoint.x, rootPoint.y, rootPoint.z );
		MFloatVector dir(firstPoint.x - rootPoint.x, firstPoint.y - rootPoint.y, firstPoint.z - rootPoint.z);
		
		MFloatPointArray hitPoints;
		bool intersect = currentMesh->allIntersections( startP, dir, 0, 0, false, MSpace::kWorld, 1, false, &accelParam, false, hitPoints, 0, 0, 0, 0, 0 );

		// clearing additional information
		bool curentPointInsideMesh = hitPoints.length() % 2;

		if(curentPointInsideMesh)
		{
			MPointOnMesh closestPointInfo;
			MStatus status = accelerator->getClosestPoint( firstPoint.toMayaPoint(), closestPointInfo );
			MVector closestPointWorld ( closestPointInfo.getPoint().x, closestPointInfo.getPoint().y, closestPointInfo.getPoint().z );
			closestPointWorld = closestPointWorld * inclusiveMatrix;

			Vec3 p( closestPointWorld.x, closestPointWorld.y, closestPointWorld.z);

			guide->mSegmentsAdditionalInfo[ 1 ].mClosestPointOnMesh = Vec3::transformPoint(p, localMatrix);
			guide->mSegmentsAdditionalInfo[ 1 ].mSurfaceDirection = (guide->mSegmentsAdditionalInfo[ 1 ].mClosestPointOnMesh - guide->mGuideSegments.mSegments[ 1 ]).normalize();
			guide->mSegmentsAdditionalInfo[ 1 ].mIsColliding = true;
			guide->mCollisionsCount++;
		}
		else
		{
			guide->mSegmentsAdditionalInfo[ 1 ].mClosestPointOnMesh.set(0.0, 0.0, 0.0);
			guide->mSegmentsAdditionalInfo[ 1 ].mIsColliding = false;
		}

		// iterating all segments
		for(unsigned i = 2; i < guide->mGuideSegments.mSegments.size(); ++i)
		{
			Vec3 positionStartPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments[ i - 1 ], worldMatrix );
			Vec3 positionEndPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments[ i ], worldMatrix );
			Vec3 positionDirection = positionEndPoint - positionStartPoint;
			
			MFloatPoint startP(positionStartPoint.x, positionStartPoint.y, positionStartPoint.z);
			MFloatVector dir(positionDirection.x, positionDirection.y, positionDirection.z);

			MFloatPointArray hitPoints;
			bool intersect = currentMesh->allIntersections( startP, dir, 0, 0, false, MSpace::kWorld, 1, false, &accelParam, false, hitPoints, 0, 0, 0, 0, 0 );

			// current segment has an intersection -> so we change hair intersection
			if(intersect && hitPoints.length() % 2)
			{
				curentPointInsideMesh = !curentPointInsideMesh;
			}

			// nearest point on mesh
			if(curentPointInsideMesh)
			{
				MPointOnMesh closestPointInfo;
				MStatus status = accelerator->getClosestPoint( positionEndPoint.toMayaPoint(), closestPointInfo );
				MVector closestPointWorld ( closestPointInfo.getPoint().x, closestPointInfo.getPoint().y, closestPointInfo.getPoint().z );
				closestPointWorld = closestPointWorld * inclusiveMatrix;

				Vec3 p( closestPointWorld.x, closestPointWorld.y, closestPointWorld.z);

				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh = Vec3::transformPoint(p, localMatrix);
				guide->mSegmentsAdditionalInfo[ i ].mSurfaceDirection = (guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh - guide->mGuideSegments.mSegments[ i ]).normalize();
				guide->mSegmentsAdditionalInfo[ i ].mIsColliding = true;
				guide->mCollisionsCount++;
			}
			else
			{
				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh.set(0.0, 0.0, 0.0);
				guide->mSegmentsAdditionalInfo[ i ].mIsColliding = false;
			}
		} // for all remaining segments
		assert ( guide->mSegmentsAdditionalInfo[ 0 ].mIsColliding == false );
		assert ( guide->mCollisionsCount <= guide->mGuideSegments.mSegments.size() - 1 );
		guide->mCollisionsCount = 0;
	} // for all guides
}

void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		const Real SCALE_FACTOR = guide->mGuideSegments.mSegmentLength;

		if (SCALE_FACTOR <= EPSILON)
		{
			continue;
		}

		const Real SEGMENT_LENGTH_SQ = 1.0; // Desired segments' length squared
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Alias for hair vertices
		const Uint VERTEX_COUNT = (Uint)hairVertices.size(); // Number of hair vertices
		Uint collisionsCount = guide->mCollisionsCount; // Number of colliding hair vertices

		assert( collisionsCount <= VERTEX_COUNT - 1 );

		Uint constraintsCount = (VERTEX_COUNT - 1) + collisionsCount; // Number of constraints
		const Uint COL_CONSTR_OFFSET = VERTEX_COUNT - 1; // Offset to the beginning of collision constraints
		const Uint DERIVATIVES_COUNT = 3 * (VERTEX_COUNT - 1); // Number of constraint derivatives

		// Rescale hair vertices before computations so all segments are of unit length
		HairTaskProcessor::rescaleGuideHair(hairVertices, 1.0 / SCALE_FACTOR);
		HairTaskProcessor::rescaleClosestPoints(guide->mSegmentsAdditionalInfo, 1.0 / SCALE_FACTOR);

		// Input vectors and matrices:
		RealN C(constraintsCount); // Constraint vector
		RealNxN NC(constraintsCount, DERIVATIVES_COUNT); // Nabla C matrix containing partial derivatives of the C vector
		RealNxN delta(DERIVATIVES_COUNT, constraintsCount); // In the original paper this is NC transpose multiplied by inverse mass matrix and time step squared
		
		// Solution vectors and matrices:
		RealN lambda; // system inverse matrix multiplied by C vector
		RealN dX; // Vector containing corrections
		RealNxN system; // NC matrix multiplied by delta matrix

		// Temporary and utility variables:
		Vec3 e; // Vector for calculating error
		Vec3 correction; // Vector for storing vertex corrections
		Uint iterationsCount = 0;
		Real absC = 0.0; // Absolute value of the constraint vector
		Real previousAbsC = 0.0; // Previous absolute value of the constraint vector for premature convergence detection
		while (true) // Repeat until converged
		{
			// -------------------------------------------------------------------------------------
			// Step 1: Update the constraint vector
			// -------------------------------------------------------------------------------------
			C = 0.0;
			HairTaskProcessor::computeInextensibilityConstraints(C, hairVertices, SEGMENT_LENGTH_SQ);

			if (collisionsCount > 0)
			{
				HairTaskProcessor::computeInterpenetrationConstraints(C, hairVertices, guide->mSegmentsAdditionalInfo, COL_CONSTR_OFFSET);
			}

			// Convergence condition:
			previousAbsC = absC;
			absC = C.MaximumAbsoluteValue();
			bool localMinimum = (previousAbsC - HairTaskProcessor::DELTA <= absC) && (absC <= previousAbsC + HairTaskProcessor::DELTA);
			if ( absC <= HairTaskProcessor::CONVERGENCE_THRESHOLD ||
				iterationsCount >= HairTaskProcessor::MAX_LOOP_ITERATIONS /*||
				localMinimum*/ )
			{
				// Rescale hair vertices to retain their original scale
				HairTaskProcessor::rescaleGuideHair(hairVertices, SCALE_FACTOR);
				HairTaskProcessor::rescaleClosestPoints(guide->mSegmentsAdditionalInfo, SCALE_FACTOR);

				//TODO: Remove me
				std::cout << iterationsCount << std::endl << std::flush;

				break;
			}
			// -------------------------------------------------------------------------------------
			// Step 2: Prepare solution matrices
			// -------------------------------------------------------------------------------------
			NC = 0.0;
			delta = 0.0;
			HairTaskProcessor::computeInextensibilityGradient(NC, delta, hairVertices);

			if (collisionsCount > 0)
			{
				HairTaskProcessor::computeInterpenetrationGradient(NC, delta, hairVertices, guide->mSegmentsAdditionalInfo, COL_CONSTR_OFFSET);
			}
			// -------------------------------------------------------------------------------------
			// Step 3: Calculate and apply position changes
			// -------------------------------------------------------------------------------------
			system = NC * delta;
			try
			{
				lambda = system.i() * C;
			}
			catch (...) // Something went terribly wrong...
			{
				// Rescale hair vertices to retain their original scale
				HairTaskProcessor::rescaleGuideHair(hairVertices, SCALE_FACTOR);
				break;
			}
			dX = -delta * lambda;

			// Apply corrections to all vertices except the first one
			Uint j = 0; // dX collision correction index
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				correction.set(dX[ 3*i ], dX[ 3*i + 1 ] , dX[ 3*i + 2 ]);
				hairVertices[ i + 1 ] += correction;
			}

			// Update the collision set - FIXME: doesn't work
			if ( collisionsCount > 0 )
			{
				//collisionsCount = HairTaskProcessor::updateCollisionInfo(hairVertices, guide->mSegmentsAdditionalInfo);
				assert( collisionsCount <= VERTEX_COUNT - 1 );
				assert( collisionsCount <= guide->mCollisionsCount );
			}
			// Resize matrices if the set has been reduced
			if ( collisionsCount < guide->mCollisionsCount )
			{
				guide->mCollisionsCount = collisionsCount;
				constraintsCount = (VERTEX_COUNT - 1) + collisionsCount;
				C.ReSize(constraintsCount);
				NC.ReSize(constraintsCount, DERIVATIVES_COUNT);
				delta.ReSize(DERIVATIVES_COUNT, constraintsCount);
			}

			iterationsCount++;
		} // while (true)
		
		// Delete information about collisions in case the user disables them
		guide->mCollisionsCount = 0;
	} // for each guide
}

void HairTaskProcessor::enforceConstraints(HairShape::HairComponents::Segments &aVertices, Real aSegmentLength)
{
	const Real SCALE_FACTOR = aSegmentLength;

	if (SCALE_FACTOR <= EPSILON)
	{
		return;
	}

	const Real SEGMENT_LENGTH_SQ = 1.0; // Desired segment length squared
	const Uint VERTEX_COUNT = (Uint)aVertices.size(); // Number of hair vertices
	const Uint CONSTRAINTS_COUNT = VERTEX_COUNT - 1; // Number of constraints
	const Uint DERIVATIVES_COUNT = 3 * (VERTEX_COUNT - 1); // Number of constraint derivatives

	// Solution vectors and matrices:
	RealN C(CONSTRAINTS_COUNT); // Constraint vector
	RealN lambda(CONSTRAINTS_COUNT); // system inverse matrix multiplied by C vector
	RealN dX(DERIVATIVES_COUNT); // Vector containing corrections
	RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // Nabla C matrix containing partial derivatives of the C vector
	RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // In the original paper this is NC transpose multiplied by inverse mass matrix and time step squared
	RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT); // NC matrix multiplied by delta matrix

	// Rescale hair vertices before computations so all segments are of unit length
	HairTaskProcessor::rescaleGuideHair(aVertices, 1.0 / SCALE_FACTOR);

	// Temporary and utility variables
	Vec3 e; // Vector for calculating error
	Vec3 correction; // Vector for storing vertex corrections
	Uint iterationsCount = 0;
	while (true) // Repeat until converged
	{
		// -------------------------------------------------------------------------------------
		// Step 1: Update the constraint vector
		// -------------------------------------------------------------------------------------
		C = 0.0;
		HairTaskProcessor::computeInextensibilityConstraints(C, aVertices, SEGMENT_LENGTH_SQ);

		// Convergence condition:
		if (C.MaximumAbsoluteValue() <= HairTaskProcessor::CONVERGENCE_THRESHOLD ||
			iterationsCount >= HairTaskProcessor::MAX_LOOP_ITERATIONS)
		{
			// Rescale hair vertices to retain their original scale
			HairTaskProcessor::rescaleGuideHair(aVertices, SCALE_FACTOR);

			break;
		}
		// -------------------------------------------------------------------------------------
		// Step 2: Prepare solution matrices
		// -------------------------------------------------------------------------------------
		NC = 0.0;
		delta = 0.0;
		HairTaskProcessor::computeInextensibilityGradient(NC, delta, aVertices);
		// -------------------------------------------------------------------------------------
		// Step 3: Calculate and apply position changes
		// -------------------------------------------------------------------------------------
		system = NC * delta;
		try
		{
			lambda = system.i() * C;
		}
		catch (...) // Something went terribly wrong...
		{
			// Rescale hair vertices to retain their original scale
			HairTaskProcessor::rescaleGuideHair(aVertices, SCALE_FACTOR);
			break;
		}
		dX = -delta * lambda;

		// Apply corrections to all vertices except the first one
		Uint j = 0; // dX collision correction index
		for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
		{
			correction.set(dX[ 3*i ], dX[ 3*i + 1 ] , dX[ 3*i + 2 ]);
			aVertices[ i + 1 ] += correction;
		}

		iterationsCount++;
	} // while (true)
}

} // namespace Toolbox

} // namespace Stubble
