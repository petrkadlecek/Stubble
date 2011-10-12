#include "HairTaskProcessor.hpp"

#include <maya\MMeshIntersector.h>

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

const Uint HairTaskProcessor::MAX_LOOP_ITERATIONS = 10;
const Real HairTaskProcessor::CONVERGENCE_THRESHOLD = 1e-4;
const Uint HairTaskProcessor::RIGID_BODY_COUPL_CONSTRAINTS = 3;
const Real HairTaskProcessor::INV_ROOT_SGMT_WEIGHT = 1.0;
const Real HairTaskProcessor::INV_MID_SGMT_WEIGHT = 2e7;
const Real HairTaskProcessor::DELTA_SCALE = 1e-6;

// ----------------------------------------------------------------------------
// Methods:
// ----------------------------------------------------------------------------

void HairTaskProcessor::waitFinishWorkerThread ()
{
	// Contains critical section
	while ( HairTaskProcessor::isRunning() )
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
		mTaskAccumulator.push_back(aTask); //TODO: exception handling?
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	// Contains critical section
	HairTaskProcessor::tryCreateWorkerThread();
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
	// Contains critical section
	if ( HairTaskProcessor::isRunning() )
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
	size_t accumulatorSize = hairTaskProcessor->getAccumulatorSize(); // Contains critical section

	while ( accumulatorSize > 0 )
	{
		// Contains critical section
		HairTask *task = 0;
		accumulatorSize = hairTaskProcessor->getTask(task);

		if ( 0 != task )
		{
			hairTaskProcessor->doBrush(*task->mAffectedGuides, task->mDx, task->mBrushMode);
			hairTaskProcessor->detectCollisions( *task->mAffectedGuides );
			hairTaskProcessor->enforceConstraints( *task->mAffectedGuides );

			//task->mParentHairShape->updateGuides( false );  //FIXME: caused changes not being stored, because it overwrote all dirty flags

			delete task;
		}
	}

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
			aTask = mTaskAccumulator.front();
			mTaskAccumulator.pop_front();
			queueSize--;
		}
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return queueSize;
}

void HairTaskProcessor::doBrush (HairShape::HairComponents::SelectedGuides &aSelectedGuides, const Vector3D< Real > &aDx, BrushMode *aBrushMode)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		aBrushMode->doBrush(aDx, **it);
	}
}

void HairTaskProcessor::detectCollisions( HairShape::HairComponents::SelectedGuides &aSelectedGuides )
{
	MMeshIntersector *accelerator =  HairShape::HairShape::getActiveObject()->getCurrentMesh()->getMeshIntersector();

	for( HairShape::HairComponents::SelectedGuides::iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
	{
		const Vector3D<Real> normal = (*it)->mNormal;
		Vector3D<double> firstPoint = (*it)->mGuideSegments.mSegments.at(1);
		Vector3D<double> rootPoint = (*it)->mGuideSegments.mSegments.at(0);

		// clearing additional informations
		Vector3D<double> r = firstPoint - rootPoint;
		bool intersected = Vector3D<double>().dotProduct(r, normal) < 0;

		// iterating all segments
		for(unsigned i = 1; i < (*it)->mGuideSegments.mSegments.size(); ++i)
		{
			Vector3D<Real> positionDirection = (*it)->mGuideSegments.mSegments[ i ] - (*it)->mGuideSegments.mSegments[ i - 1 ];
			Vector3D<Real> positionStart = (*it)->mGuideSegments.mSegments[ i - 1 ];

			MFloatPoint startP(positionStart.x, positionStart.y, positionStart.z);
			MFloatVector dir(positionDirection.x, positionDirection.y, positionDirection.z);

			MFloatPoint hitPoint;
			MFnMesh * currentMesh = HairShape::HairShape::getActiveObject()->getCurrentMesh()->getMayaMesh();
			MMeshIsectAccelParams accelParam = currentMesh->autoUniformGridParams();

			bool intersect = currentMesh->anyIntersection( startP, dir, 0, 0, false, MSpace::kWorld, 1, false, &accelParam,
				hitPoint, 0, 0, 0, 0, 0 );

			(*it)->mSegmentsAdditionalInfo[ i ].mIsColliding = intersected;

			// nearest point on mesh
			if(intersect)
			{
				MPointOnMesh closest;
				MPoint queryPoint( startP );
				accelerator->getClosestPoint( queryPoint, closest, MSpace::kWorld);
				(*it)->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh = Vector3D<Real>( closest.getPoint().x, closest.getPoint().y, closest.getPoint().z );

				intersected = !intersected;
			}
			else
				(*it)->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh = Vector3D<Real>();

		}
	}
}

void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		const Real SEGMENT_LENGTH_SQ = (*it)->mGuideSegments.mSegmentLength * (*it)->mGuideSegments.mSegmentLength;
		HairShape::HairComponents::Segments &hairVertices = (*it)->mGuideSegments.mSegments;
		const Uint VERTEX_COUNT = ( Uint )hairVertices.size();
		Vec3 folliclePosition(0.0, 0.0, 0.0); // Virtual follicle position, ideally this shouldn't move, but that makes the equation system below unstable

		const Uint CONSTRAINTS_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + VERTEX_COUNT - 1;
		const Uint DERIVATIVES_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + 3 * VERTEX_COUNT;

		RealN C(CONSTRAINTS_COUNT);

		Uint iterationsCount = 0;

		while (true) // Repeat until converged
		{
			// -------------------------------------------------------------------------------------
			// Step 1: Update the constraint vector
			// -------------------------------------------------------------------------------------
			C = 0.0;
			// Rigid body coupling constraints:
			Vec3 diff = folliclePosition - hairVertices[ 0 ];
			for (Uint i = 0; i < RIGID_BODY_COUPL_CONSTRAINTS; ++i)
			{
				C[ i ] = diff[ i ];
			}

			// Inextensibility constraints:
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				Vec3 e = hairVertices[ i + 1 ] - hairVertices[ i ];
				C[ RIGID_BODY_COUPL_CONSTRAINTS + i ] = Vec3::dotProduct(e, e) - SEGMENT_LENGTH_SQ;
			}

			//TODO: debug - remove me
			/*std::cout << "C = ";
			for (Uint i = 0; i < CONSTRAINTS_COUNT; ++i)
			{
				std::cout << C[ i ] << " ";
			}
			std::cout << std::endl << std::flush;*/
			//break;
			// ------------------------

			// Convergence condition:
			if (C.MaximumAbsoluteValue() <= CONVERGENCE_THRESHOLD || iterationsCount >= MAX_LOOP_ITERATIONS)
			{
				break;
			}

			// -------------------------------------------------------------------------------------
			// Step 2: Prepare solution matrices
			// -------------------------------------------------------------------------------------
			RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // The "nabla C" matrix defining a direction of the constraint force
			NC = 0.0;
			RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // The delta matrix - a scaled NC transpose premultiplied by the inversed mass matrix (weights)
			delta = 0.0;

			// Rigid body coupling constraint derivatives:
			for (Uint i = 0; i < RIGID_BODY_COUPL_CONSTRAINTS; ++i)
			{
				NC[ i ][ i ] = 1.0;
				NC[ i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] = -1.0;

				delta[ i ][ i ] = NC[ i ][ i ] * DELTA_SCALE * INV_ROOT_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ i ] = NC[ i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] * DELTA_SCALE * INV_ROOT_SGMT_WEIGHT;
			}

			// Inextensibility constraint derivatives:
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				Vec3 e = (hairVertices[ i + 1 ] - hairVertices[ i ]) * 2.0;

				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ] = -e.x;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) ] = e.x;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ] = -e.y;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 1 ] = e.y;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ] = -e.z;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 2 ] = e.z;

				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 1 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 1 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 2 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i + 1) + 2 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
			}

			// Solve the equation - just part of the one from the article, this one omits velocity changes and other dynamic stuff
			RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT);
			system = NC * delta;
			
			RealN lambda(CONSTRAINTS_COUNT);
			lambda = system.i() * C;

			// -------------------------------------------------------------------------------------
			// Step 3: Calculate and apply position changes
			// -------------------------------------------------------------------------------------
			RealN dX(DERIVATIVES_COUNT); // Position change vector
			dX = -delta * lambda;

			Vec3 change(dX[ 0 ], dX[ 1 ], dX[ 2 ]);
			folliclePosition += change; // Update just the virtual position

			for (Uint i = 0; i < VERTEX_COUNT; ++i)
			{
				change.set(dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ], dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1], dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2]);

				hairVertices[ i ] += change;
			}

			iterationsCount++;
		} // while (true)
	} // for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
}

} // namespace Toolbox

} // namespace Stubble