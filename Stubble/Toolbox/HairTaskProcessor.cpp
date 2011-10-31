#include <fstream> //TODO: remove me
#include "HairTaskProcessor.hpp"
#include <maya\MMeshIntersector.h>

//#define STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE 1

namespace Stubble
{

namespace Toolbox
{

//TODO: remove me
void dumpToFile(const NEWMAT::Matrix &M, Uint rows, Uint cols)
{
	std::fstream log("C:\\Dev\\matrix.log", std::ios_base::app);
	log.setf(std::ios::fixed, std::ios::floatfield);
	log.precision(8);
	for (Uint i = 0; i < rows; ++i)
	{
		for (Uint j = 0; j < cols; ++j)
		{
			log << M[ i ][ j ] << '\t';
		}
		log << "\n";
	}
	log << "\n" << std::flush;
	log.close();
}

void dumpToFile(const NEWMAT::ColumnVector &C, Uint size)
{
	std::fstream log("C:\\Dev\\vector.log", std::ios_base::app);
	log.setf(std::ios::fixed, std::ios::floatfield);
	log.precision(8);
	log << "|C| = " << C.MaximumAbsoluteValue() << ";";
	for (Uint i = 0; i < size; ++i)
	{
		log << "\t" << C[ i ];
	}
	log << "\n" << std::flush;
	log.close();
}
//end of debug code
//----------------------

// ----------------------------------------------------------------------------
// Static data members and constants:
// ----------------------------------------------------------------------------

HairTaskProcessor *HairTaskProcessor::sInstance = 0;
bool HairTaskProcessor::sIsRunning = false;
MSpinLock HairTaskProcessor::sIsRunningLock;

const Uint HairTaskProcessor::MAX_LOOP_ITERATIONS = 10;
const Real HairTaskProcessor::CONVERGENCE_THRESHOLD = 1e-8;
const Real HairTaskProcessor::EPSILON = 1e-5;
#ifdef STUBBLE_ORIGINAL_HAIRSTYLING
const Uint HairTaskProcessor::RIGID_BODY_COUPL_CONSTRAINTS = 0;
const Real HairTaskProcessor::INV_ROOT_SGMT_WEIGHT = 1.0;
const Real HairTaskProcessor::INV_MID_SGMT_WEIGHT = 2e7;
const Real HairTaskProcessor::DELTA_SCALE = 1e-6;
#endif


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
		//std::cout << "Accumulator size = " << accumulatorSize << std::endl << std::flush; //TODO: remove me

		// Contains critical section
		HairTask *task = 0;
		accumulatorSize = hairTaskProcessor->getTask(task);

		if ( 0 != task )
		{
			hairTaskProcessor->doBrush(task);
			if ( task->mBrushMode->isCollisionDetectionEnabled() )
			{
				hairTaskProcessor->detectCollisions( *task->mAffectedGuides );
			}
			HairTaskProcessor::enforceConstraints( *task->mAffectedGuides );

			task->mParentHairShape->updateGuides( false );

			delete task;
		}
	}

	//std::cout << "Ending loop..." << std::endl << std::flush; //TODO: remove me

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

void HairTaskProcessor::doBrush (HairTask *aTask)
{
	Vector3D< Real > dX = aTask->mBrushMode->preBrushTransform(aTask->mDx, (void *)(&aTask->mView));
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		aTask->mBrushMode->doBrush(dX, **it);
		(*it)->mDirtyFlag = true;
		(*it)->mDirtyRedrawFlag = true;
	}
}

void HairTaskProcessor::detectCollisions( HairShape::HairComponents::SelectedGuides &aSelectedGuides )
{
	MMeshIntersector *accelerator =  HairShape::HairShape::getActiveObject()->getCurrentMesh()->getMeshIntersector();
	MFnMesh *currentMesh = HairShape::HairShape::getActiveObject()->getCurrentMesh()->getMayaMesh();

	for( HairShape::HairComponents::SelectedGuides::iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
	{
		HairShape::HairComponents::SelectedGuide *guide = *it;
		guide->mCollisionsCount = 0;

		Matrix< Real > &worldMatrix = guide->mPosition.mWorldTransformMatrix;
		Matrix< Real > &localMatrix = guide->mPosition.mLocalTransformMatrix;

		const Vec3 normal = guide->mNormal;
		Vec3 firstPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(1), worldMatrix );
		Vec3 rootPoint = Vec3::transformPoint( guide->mGuideSegments.mSegments.at(0), worldMatrix );

		// clearing additional informations
		Vec3 r = firstPoint - rootPoint;
		bool curentPointInsideMesh = Vec3::dotProduct(r, normal) < 0;

		if(curentPointInsideMesh)
		{
			MPoint closest;
			MPoint queryPoint( firstPoint.x, firstPoint.y, firstPoint.z );
			currentMesh->getClosestPoint( queryPoint, closest, MSpace::kWorld );
			//accelerator->getClosestPoint( queryPoint, closest );
			Vec3 p( closest.x, closest.y, closest.z );

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

			MFloatPoint hitPoint;
			MMeshIsectAccelParams accelParam = currentMesh->autoUniformGridParams();
			bool intersect = currentMesh->anyIntersection( startP, dir, 0, 0, false, MSpace::kWorld, 1, false, &accelParam,	hitPoint, 0, 0, 0, 0, 0 );

			// current segment has an intersection -> so we change hair intersection
			if(intersect)
				curentPointInsideMesh = !curentPointInsideMesh;

			// nearest point on mesh
			if(curentPointInsideMesh)
			{
				MPoint closest;
				MPoint queryPoint( positionEndPoint.x, positionEndPoint.y, positionEndPoint.z );
				currentMesh->getClosestPoint( queryPoint, closest, MSpace::kWorld );
				bool ic = accelerator->isCreated();

				MPointOnMesh qqq;
				accelerator->getClosestPoint( queryPoint, qqq );

				Vec3 p( closest.x, closest.y, closest.z );
				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh = Vec3::transformPoint(p, localMatrix);
			}
			else
			{
				guide->mSegmentsAdditionalInfo[ i ].mClosestPointOnMesh.set(0.0, 0.0, 0.0);
				//guide->mSegmentsAdditionalInfo[ i ].mIsColliding = false;
			}

			guide->mSegmentsAdditionalInfo[ i ].mIsColliding = curentPointInsideMesh;
			guide->mCollisionsCount += (curentPointInsideMesh) ? 1 : 0;
		} // for segments from 2 to end
		//std::cout << "# of collisions = " << guide->mCollisionsCount << std::endl << std::flush; //TODO: remove me
		//guide->mCollisionsCount = 0; //TODO: Remove me
	} // for all guides
}

#ifdef STUBBLE_ORIGINAL_HAIRSTYLING
void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		Vec3 folliclePosition(0.0, 0.0, 0.0); // Virtual follicle position, ideally this shouldn't move, but that makes the equation system below unstable
		const Real SEGMENT_LENGTH_SQ = (*it)->mGuideSegments.mSegmentLength * (*it)->mGuideSegments.mSegmentLength; // Squared segment's length
		HairShape::HairComponents::Segments &hairVertices = (*it)->mGuideSegments.mSegments;
		const Uint VERTEX_COUNT = ( Uint )hairVertices.size();

#ifdef STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE
		HairShape::HairComponents::SegmentsAdditionalInfo &hairVerticesInfo = (*it)->mSegmentsAdditionalInfo;
		assert( VERTEX_COUNT == (Uint)hairVerticesInfo.size());
#endif

		

#ifndef STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE
		const Uint CONSTRAINTS_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + VERTEX_COUNT - 1;
		const Uint DERIVATIVES_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + 3 * VERTEX_COUNT - 3;
#else
		const Uint CONSTRAINTS_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + 2 * (VERTEX_COUNT - 1);
		const Uint COL_CONSTR_OFFSET = RIGID_BODY_COUPL_CONSTRAINTS + VERTEX_COUNT - 1; // Offset to the beginning of collision constraints
		const Uint DERIVATIVES_COUNT = RIGID_BODY_COUPL_CONSTRAINTS + 3 * VERTEX_COUNT + VERTEX_COUNT + 2;
		const Uint COL_DERIV_OFFSET = RIGID_BODY_COUPL_CONSTRAINTS + 3 * VERTEX_COUNT; // Offset to the beginning of collision constraint derivatives
#endif

		RealN C(CONSTRAINTS_COUNT);

		Uint iterationsCount = 0;

		while (true) // Repeat until converged
		{
			// -------------------------------------------------------------------------------------
			// Step 1: Update the constraint vector
			// -------------------------------------------------------------------------------------
			C = 0.0;
			// Rigid body coupling constraints:
			/*Vec3 diff = folliclePosition - hairVertices[ 0 ];
			for (Uint i = 0; i < RIGID_BODY_COUPL_CONSTRAINTS; ++i)
			{
				C[ i ] = diff[ i ];
			}*/

			// Inextensibility constraints:
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				Vec3 e = hairVertices[ i + 1 ] - hairVertices[ i ];
				C[ RIGID_BODY_COUPL_CONSTRAINTS + i ] = Vec3::dotProduct(e, e) - SEGMENT_LENGTH_SQ;
			}

#ifdef STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE
			// Collision constraints:
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				Real error = 0.0;
				if ( hairVerticesInfo[ i + 1 ].mIsColliding ) // We don't check the root vertex for obvious reasons
				{
					Vec3 e = hairVertices[ i + 1 ] - hairVerticesInfo[ i + 1 ].mClosestPointOnMesh;
					error = e.size();
				}
				C[ COL_CONSTR_OFFSET + i ] = error;
			}
#endif

			//TODO: debug - remove me
			/*std::cout << "|C| = " << C.MaximumAbsoluteValue();
			std::cout << ", C = ";
			for (Uint i = 0; i < CONSTRAINTS_COUNT; ++i)
			{
				std::cout << C[ i ] << " ";
			}
			std::cout << std::endl << std::flush;*/
			//break;
			// ------------------------

			// Convergence condition:
			if (C.MaximumAbsoluteValue() <= CONVERGENCE_THRESHOLD /*|| iterationsCount >= MAX_LOOP_ITERATIONS*/)
			{
				/*Vec3 correction = Vec3(0.0, 0.0, 0.0) - hairVertices[ 0 ];
				for (Uint i = 0; i < VERTEX_COUNT; ++i)
				{
					hairVertices[ i ] += correction;
				}*/

				std::cout << "# of iterations = " << iterationsCount << std::endl << std::flush;

				break;
			}

			// -------------------------------------------------------------------------------------
			// Step 2: Prepare solution matrices
			// -------------------------------------------------------------------------------------
			RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // The "nabla C" matrix defining a direction of the constraint force
			NC = 0.0;
			RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // The delta matrix - a scaled NC transpose pre-multiplied by the inversed mass matrix (weights)
			delta = 0.0;

			// Rigid body coupling constraint derivatives:
			/*for (Uint i = 0; i < RIGID_BODY_COUPL_CONSTRAINTS; ++i)
			{
				NC[ i ][ i ] = 1.0;
				NC[ i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] = -1.0;

				delta[ i ][ i ] = NC[ i ][ i ] * DELTA_SCALE * INV_ROOT_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ i ] = NC[ i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] * DELTA_SCALE * INV_ROOT_SGMT_WEIGHT;
			}*/

			// Inextensibility constraint derivatives:
			Vec3 e = (hairVertices[ 1 ] - hairVertices[ 0 ]) * 2.0;
			NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS ] = e.x;
			NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS + 1 ] = e.y;
			NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS + 2 ] = e.z;

			delta[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS ] =
				NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
			delta[ RIGID_BODY_COUPL_CONSTRAINTS + 1 ][ RIGID_BODY_COUPL_CONSTRAINTS ] =
				NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS + 1 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
			delta[ RIGID_BODY_COUPL_CONSTRAINTS + 2 ][ RIGID_BODY_COUPL_CONSTRAINTS ] =
				NC[ RIGID_BODY_COUPL_CONSTRAINTS ][ RIGID_BODY_COUPL_CONSTRAINTS + 2 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;

			for (Uint i = 1; i < VERTEX_COUNT - 1; ++i)
			{
				e = (hairVertices[ i + 1 ] - hairVertices[ i ]) * 2.0;

				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) ] = -e.x;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ] = e.x;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 1 ] = -e.y;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ] = e.y;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 2 ] = -e.z;
				NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ] = e.z;

				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 1 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 1 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 2 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*(i - 1) + 2 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ][ RIGID_BODY_COUPL_CONSTRAINTS + i ] =
					NC[ RIGID_BODY_COUPL_CONSTRAINTS + i ][ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
			}

#ifdef STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE
			// Collision constraint derivatives:
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{
				NC[ COL_CONSTR_OFFSET + i ][ COL_DERIV_OFFSET + i ] = 1.0;
				NC[ COL_CONSTR_OFFSET + i ][ COL_DERIV_OFFSET + i + 3 ] = 1.0;

				delta[ COL_DERIV_OFFSET + i ][ COL_CONSTR_OFFSET + i ] =
					NC[ COL_CONSTR_OFFSET + i ][ COL_DERIV_OFFSET + i ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
				delta[ COL_DERIV_OFFSET + i + 3 ][ COL_CONSTR_OFFSET + i ] =
					NC[ COL_CONSTR_OFFSET + i ][ COL_DERIV_OFFSET + i + 3 ] * DELTA_SCALE * INV_MID_SGMT_WEIGHT;
			}
#endif
			//dumpToFile(NC, CONSTRAINTS_COUNT, DERIVATIVES_COUNT); //TODO: remove me
			//dumpToFile(delta, DERIVATIVES_COUNT, CONSTRAINTS_COUNT); //TODO: remove me

			// Solve the equation - just part of the one from the article, this one omits velocity changes and other dynamic stuff
			RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT);
			system = NC * delta;
			
			//dumpToFile(system, CONSTRAINTS_COUNT, CONSTRAINTS_COUNT); //TODO: remove me

			RealN lambda(CONSTRAINTS_COUNT);
			lambda = system.i() * C;

			// -------------------------------------------------------------------------------------
			// Step 3: Calculate and apply position changes
			// -------------------------------------------------------------------------------------
			RealN dX(DERIVATIVES_COUNT); // Position change vector
			dX = -delta * lambda;

			// Apply change to coupled rigid bodies
			//Vec3 change(dX[ 0 ], dX[ 1 ], dX[ 2 ]);
			//folliclePosition += change; // Update just the virtual position - mesh won't be moved
			Vec3 change;

			// Apply change to all hair vertices
			for (Uint i = 0; i < VERTEX_COUNT - 1; ++i)
			{

#ifndef STUBBLE_CONSTRAINT_BASED_COLLISION_RESPONSE
				change.set(dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ], dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1], dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2]);
#else
				change.set(dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i ] + dX[ COL_DERIV_OFFSET + 3*i ],
					dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 1] + dX[ COL_DERIV_OFFSET + 3*i + 1 ],
					dX[ RIGID_BODY_COUPL_CONSTRAINTS + 3*i + 2] + dX[ COL_DERIV_OFFSET + 3*i + 2]);
#endif

				hairVertices[ i + 1 ] += change;
			}

			iterationsCount++;
		} // while (true)
	} // for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
}
#else
void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		const Real SEGMENT_LENGTH_SQ = guide->mGuideSegments.mSegmentLength * guide->mGuideSegments.mSegmentLength; // Desired segments' length squared
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Alias for hair vertices
		const Uint VERTEX_COUNT = (Uint)hairVertices.size(); // Number of hair vertices
		const Uint COLLISIONS_COUNT = guide->mCollisionsCount; // Number of colliding hair vertices
		assert( COLLISIONS_COUNT <= VERTEX_COUNT - 1 );
		const Uint CONSTRAINTS_COUNT = (VERTEX_COUNT - 1) + COLLISIONS_COUNT; // Number of constraints
		const Uint COL_CONSTR_OFFSET = VERTEX_COUNT - 1; // Offset to the beginning of collision constraints
		const Uint DERIVATIVES_COUNT = 3 * (VERTEX_COUNT - 1) + 3 * COLLISIONS_COUNT; // Number of constraint derivatives
		const Uint COL_DERIV_OFFSET = 3 * (VERTEX_COUNT - 1); // Offset to the beginning of collision constraint derivatives

		// Solution vectors
		RealN C(CONSTRAINTS_COUNT); // Constraint vector
		RealN lambda(CONSTRAINTS_COUNT); // system inverse matrix multiplied by C vector
		RealN dX(DERIVATIVES_COUNT); // Vector containing corrections
		RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // Nabla C matrix containing partial derivatives of the C vector
		RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // In the original paper this is NC transpose multiplied by inverse mass matrix and time step squared
		RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT); // NC matrix multiplied by delta matrix

		// Temporary and utility variables
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

				//TODO: debug - remove me
				/*std::cout << "|C| = " << C.MaximumAbsoluteValue();
				std::cout << ", C = ";
				for (Uint i = 0; i < CONSTRAINTS_COUNT; ++i)
				{
					std::cout << C[ i ] << " ";
				}
				std::cout << std::endl << std::flush;*/
				//break;
				// ------------------------
			}
			//dumpToFile(C, CONSTRAINTS_COUNT); //TODO: remove me

			// Convergence condition:
			previousAbsC = absC;
			absC = C.MaximumAbsoluteValue();
			if (absC <= CONVERGENCE_THRESHOLD  || (previousAbsC - EPSILON <= absC && absC <= previousAbsC + EPSILON)/*|| iterationsCount >= MAX_LOOP_ITERATIONS*/)
			{
				//std::cout << "# of iterations = " << iterationsCount << std::endl << std::flush; //TODO: remove me
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
			//dumpToFile(NC, CONSTRAINTS_COUNT, DERIVATIVES_COUNT);
			system = NC * delta;
			try
			{
				lambda = system.i() * C;
			}
			catch (NEWMAT::SingularException exception)
			{
				//dumpToFile(NC, CONSTRAINTS_COUNT, DERIVATIVES_COUNT);
				//dumpToFile(delta, DERIVATIVES_COUNT, CONSTRAINTS_COUNT);
				//dumpToFile(system, CONSTRAINTS_COUNT, CONSTRAINTS_COUNT);
				std::cout << "Matice se zpucmeloudila.\n" << std::flush;
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
	} // for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
}
#endif

void HairTaskProcessor::enforceConstraints(HairShape::HairComponents::Segments &aVertices, Real aSegmentLength)
{
	const Real SEGMENT_LENGTH_SQ = aSegmentLength * aSegmentLength; // Segment length squared
	const Uint VERTEX_COUNT = (Uint)aVertices.size(); // Number of hair vertices
	const Uint CONSTRAINTS_COUNT = VERTEX_COUNT - 1; // Number of constraints
	const Uint DERIVATIVES_COUNT = 3 * (VERTEX_COUNT - 1); // Number of constraint derivatives

	// Solution vectors
	RealN C(CONSTRAINTS_COUNT); // Constraint vector
	RealN lambda(CONSTRAINTS_COUNT); // system inverse matrix multiplied by C vector
	RealN dX(DERIVATIVES_COUNT); // Vector containing corrections
	RealNxN NC(CONSTRAINTS_COUNT, DERIVATIVES_COUNT); // Nabla C matrix containing partial derivatives of the C vector
	RealNxN delta(DERIVATIVES_COUNT, CONSTRAINTS_COUNT); // In the original paper this is NC transpose multiplied by inverse mass matrix and time step squared
	RealNxN system(CONSTRAINTS_COUNT, CONSTRAINTS_COUNT); // NC matrix multiplied by delta matrix

	// Temporary and utility variables
	Vec3 e; // Vector for calculating error
	Vec3 correction; // Vector for storing vertex corrections
	Uint iterationsCount = 0;
	while (true)// Repeat until converged
	{
		// -------------------------------------------------------------------------------------
		// Step 1: Update the constraint vector
		// -------------------------------------------------------------------------------------
		C = 0.0;
		HairTaskProcessor::computeInextensibilityConstraints(C, aVertices, SEGMENT_LENGTH_SQ);

		// Convergence condition:
		if (C.MaximumAbsoluteValue() <= CONVERGENCE_THRESHOLD  || iterationsCount >= MAX_LOOP_ITERATIONS)
		{
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
		catch (NEWMAT::SingularException exception)
		{
			std::cout << "Matice se zpucmeloudila.\n" << std::flush;
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