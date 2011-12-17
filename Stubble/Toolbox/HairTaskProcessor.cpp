#include "HairTaskProcessor.hpp"
#include "HairShape\UserInterface\HairShape.hpp"
#include <maya\MMeshIntersector.h>
#include <maya\MFloatPointArray.h>

#include <algorithm>
#include "Common/StubbleTimer.hpp" //TODO: remove me
#include <iostream>

namespace Stubble
{

namespace Toolbox
{

Timer timer; //TODO: remove me

// ----------------------------------------------------------------------------
// Static data members and constants:
// ----------------------------------------------------------------------------

HairTaskProcessor *HairTaskProcessor::sInstance = 0;
bool HairTaskProcessor::sIsRunning = false;
MSpinLock HairTaskProcessor::sIsRunningLock;

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
	if ( HairTaskProcessor::isRunning() ) // Contains critical section
	{
		return;
	}

	MStatus status = MThreadAsync::init();
	if ( MStatus::kSuccess != status )
	{
		status.perror( "HairTaskProcessor: Failed to acquire thread resources" );
		//TODO: exception?
		return;
	}

	status = MThreadAsync::createTask(HairTaskProcessor::asyncWorkerLoop, 0, HairTaskProcessor::workerFinishedCB, 0);
	if ( MStatus::kSuccess != status )
	{
		status.perror( "HairTaskProcessor: Failed to run the worker thread" );
		//TODO: exception?
	}

	return;
}

void HairTaskProcessor::workerFinishedCB (void *aData)
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::sIsRunningLock.lock();
		HairTaskProcessor::sIsRunning = false;
	HairTaskProcessor::sIsRunningLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------
}

MThreadRetVal HairTaskProcessor::asyncWorkerLoop (void *aData)
{
	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	HairTaskProcessor::sIsRunningLock.lock();
		HairTaskProcessor::sIsRunning = true;
	HairTaskProcessor::sIsRunningLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	HairTaskProcessor *hairTaskProcessor = HairTaskProcessor::getInstance();
	HairTask *task = 0;
	size_t accumulatorSize = hairTaskProcessor->getTask(task); // Contains critical section

	do
	{
		if ( 0 == task )
		{
			continue;
		}
		//timer.start();
		HairTaskProcessor::processTask( task );
		//timer.stop();
		//timer.mayaDisplayLastElapsedTime();
		delete task;
		task = 0;

		accumulatorSize = hairTaskProcessor->getTask(task); // Contains critical section
	}
	while ( accumulatorSize > 0 );

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
			//aTask = mTaskAccumulator.front();
			//mTaskAccumulator.pop_front();
			//queueSize--;
			aTask = accumulate();
			//size_t ds = queueSize; //TODO: remove me
			queueSize = mTaskAccumulator.size();
			//ds -= queueSize; //TODO: remove me
			//std::cout << "Accumulating: " << ds << ", remaining: " << queueSize << ", |dX| = " << aTask->mDx.size() << std::endl << std::flush;
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
	MMatrix inclusiveMatrix = HairShape::HairShape::getActiveObject()->getCurrentInclusiveMatrix();	
	MMeshIsectAccelParams accelParam = currentMesh->autoUniformGridParams();

	for( HairShape::HairComponents::SelectedGuides::iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
	{
		HairShape::HairComponents::SelectedGuide *guide = *it;
		guide->mCollisionsCount = 0;

		if (guide->mGuideSegments.mSegmentLength <= EPSILON)
		{
			continue;
		}

		Matrix< Real > &worldMatrix = guide->mPosition.mWorldTransformMatrix;
		Matrix< Real > &localMatrix = guide->mPosition.mLocalTransformMatrix;

		const Vec3 normal = guide->mNormal;
		Vec3 firstPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(1), worldMatrix );
		Vec3 rootPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(0), worldMatrix ) + normal * 0.01f;

		MFloatPoint startP(rootPoint.x, rootPoint.y, rootPoint.z );
		MFloatVector dir(firstPoint.x - rootPoint.x, firstPoint.y - rootPoint.y, firstPoint.z - rootPoint.z);
		
		MFloatPointArray hitPoints;
		bool intersect = currentMesh->allIntersections( startP, dir, 0, 0, false, MSpace::kWorld, 1, false, &accelParam, false, hitPoints, 0, 0, 0, 0, 0 );

		// clearing additional information
		bool curentPointInsideMesh = hitPoints.length() % 2;

		if(curentPointInsideMesh)
		{
			MPointOnMesh closestPoint;
			MStatus status = accelerator->getClosestPoint( firstPoint.toMayaPoint(), closestPoint );

			Vec3 p( closestPoint.getPoint().x + inclusiveMatrix.matrix[3][0],
				closestPoint.getPoint().y + inclusiveMatrix.matrix[3][1],
				closestPoint.getPoint().z + inclusiveMatrix.matrix[3][2]);

			guide->mSegmentsAdditionalInfo[ 1 ].mClosestPointOnMesh = Vec3::transformPoint(p, localMatrix);
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
				curentPointInsideMesh = !curentPointInsideMesh;

			// nearest point on mesh
			if(curentPointInsideMesh)
			{
				MPointOnMesh closestPoint;
				MStatus status = accelerator->getClosestPoint( positionEndPoint.toMayaPoint(), closestPoint );

				Vec3 p( closestPoint.getPoint().x + inclusiveMatrix.matrix[3][0],
					closestPoint.getPoint().y + inclusiveMatrix.matrix[3][1],
					closestPoint.getPoint().z + inclusiveMatrix.matrix[3][2]);

				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh = Vec3::transformPoint(p, localMatrix);
			}
			else
			{
				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh.set(0.0, 0.0, 0.0);
			}

			guide->mSegmentsAdditionalInfo[ i ].mIsColliding = curentPointInsideMesh;
			guide->mCollisionsCount += (curentPointInsideMesh) ? 1 : 0;
		}
	}
}

void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	#pragma omp parallel for schedule( guided )
	for (__int64 guideIndex = 0; guideIndex < static_cast< __int64 >(aSelectedGuides.size()); ++guideIndex)
	//for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		//HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::SelectedGuide *guide = aSelectedGuides[ guideIndex ];
		const Real SCALE_FACTOR = guide->mGuideSegments.mSegmentLength;

		if (SCALE_FACTOR <= EPSILON)
		{
			continue;
		}

		const Real SEGMENT_LENGTH_SQ = 1.0; // Desired segments' length squared
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Alias for hair vertices
		const Uint VERTEX_COUNT = (Uint)hairVertices.size(); // Number of hair vertices
		const Uint COLLISIONS_COUNT = guide->mCollisionsCount; // Number of colliding hair vertices

		assert( COLLISIONS_COUNT <= VERTEX_COUNT - 1 );

		const Uint CONSTRAINTS_COUNT = (VERTEX_COUNT - 1) + COLLISIONS_COUNT; // Number of constraints
		const Uint COL_CONSTR_OFFSET = VERTEX_COUNT - 1; // Offset to the beginning of collision constraints
		const Uint DERIVATIVES_COUNT = 3 * (VERTEX_COUNT - 1) + 3 * COLLISIONS_COUNT; // Number of constraint derivatives
		const Uint COL_DERIV_OFFSET = 3 * (VERTEX_COUNT - 1); // Offset to the beginning of collision constraint derivatives

		// Rescale hair vertices before computations so all segments are of unit length
		HairTaskProcessor::rescaleGuideHair(hairVertices, 1.0 / SCALE_FACTOR);
		HairTaskProcessor::rescaleClosestPoints(guide->mSegmentsAdditionalInfo, 1.0 / SCALE_FACTOR);

		// Solution vectors and matrices:
		RealN C(CONSTRAINTS_COUNT); // Constraint vector
		RealN lambda(CONSTRAINTS_COUNT); // system inverse matrix multiplied by C vector
		RealN dX(DERIVATIVES_COUNT); // Vector containing corrections
		RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // Nabla C matrix containing partial derivatives of the C vector
		RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // In the original paper this is NC transpose multiplied by inverse mass matrix and time step squared
		RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT); // NC matrix multiplied by delta matrix

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

			if (COLLISIONS_COUNT > 0)
			{
				HairTaskProcessor::computeInterpenetrationConstraints(C, hairVertices, guide->mSegmentsAdditionalInfo, COL_CONSTR_OFFSET);
			}

			// Convergence condition:
			previousAbsC = absC;
			absC = C.MaximumAbsoluteValue();
			bool localMinimum = (previousAbsC - HairTaskProcessor::DELTA <= absC) && (absC <= previousAbsC + HairTaskProcessor::DELTA);
			if ( absC <= HairTaskProcessor::CONVERGENCE_THRESHOLD ||
				iterationsCount >= HairTaskProcessor::MAX_LOOP_ITERATIONS ||
				localMinimum )
			{
				// Rescale hair vertices to retain their original scale
				HairTaskProcessor::rescaleGuideHair(hairVertices, SCALE_FACTOR);

				//TODO: Remove me
				/*if (iterationsCount >= MAX_LOOP_ITERATIONS)
				{
					std::cout << "Maximum iterations reached!" << std::endl << std::flush;
				}*/

				break;
			}
			// -------------------------------------------------------------------------------------
			// Step 2: Prepare solution matrices
			// -------------------------------------------------------------------------------------
			NC = 0.0;
			delta = 0.0;
			HairTaskProcessor::computeInextensibilityGradient(NC, delta, hairVertices);

			if (COLLISIONS_COUNT > 0)
			{
				HairTaskProcessor::computeInterpenetrationGradient(NC, delta, hairVertices, guide->mSegmentsAdditionalInfo, COL_CONSTR_OFFSET, COL_DERIV_OFFSET);
			}
			// -------------------------------------------------------------------------------------
			// Step 3: Calculate and apply position changes
			// -------------------------------------------------------------------------------------
			system = NC * delta;
			try
			{
				lambda = system.i() * C;
			}
			catch (NEWMAT::SingularException)
			{
				//TODO: error log?
				break;
			}
			dX = -delta * lambda;

			// Apply corrections to all vertices except the first one
			Uint j = 0; // dX collision correction index
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				// Inextensibility correction:
				correction.set(dX[ 3*i ], dX[ 3*i + 1 ] , dX[ 3*i + 2 ]);

				// Interpenetration correction:
				if (COLLISIONS_COUNT > 0 && guide->mSegmentsAdditionalInfo[ i + 1 ].mIsColliding)
				{
					correction += Vec3(dX[ COL_DERIV_OFFSET + 3*j ], dX[ COL_DERIV_OFFSET + 3*j + 1 ], dX[ COL_DERIV_OFFSET + 3*j + 2 ] );
					++j;
				}

				hairVertices[ i + 1 ] += correction;
			}

			iterationsCount++;
		} // while (true)
		guide->mCollisionsCount = 0; // Delete information about collisions in case the user disables them
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
		catch (NEWMAT::SingularException)
		{
			//TODO: error log?
			break;
		}
		dX = -delta * lambda;

		// Apply corrections to all vertices except the first one
		Uint j = 0; // dX collision correction index
		for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
		{
			// Inextensibility correction:
			correction.set(dX[ 3*i ], dX[ 3*i + 1 ] , dX[ 3*i + 2 ]);
			aVertices[ i + 1 ] += correction;
		}

		iterationsCount++;
	} // while (true)
}

} // namespace Toolbox

} // namespace Stubble
