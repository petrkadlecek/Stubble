#include "HairTaskProcessor.hpp"

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members and constatns:
// ----------------------------------------------------------------------------

HairTaskProcessor *HairTaskProcessor::sInstance = 0;
bool HairTaskProcessor::sIsRunning = false;
MSpinLock HairTaskProcessor::sIsRunningLock;

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
	bool run = (accumulatorSize > 0); // Loop control

	while ( run )
	{
		// Contains critical section
		HairTask *task = hairTaskProcessor->getTask();
		if (task->mValid)
		{
			HairShape::HairComponents::SelectedGuides selectedGuides;
			task->mSelectedGuidesUG->select(task->mToolShape, selectedGuides);
			
			hairTaskProcessor->doBrush(selectedGuides, task->mDx, task->mBrushMode);
			hairTaskProcessor->enforceConstraints(selectedGuides);
		}
		delete task;

		// Contains critical section
		accumulatorSize = hairTaskProcessor->getAccumulatorSize();
		run = (accumulatorSize > 0);
	}

	return 0;
}

HairTask *HairTaskProcessor::getTask ()
{
	HairTask *task = 0;

	// ------------------------------------
	// Begin critical section
	// ------------------------------------
	mTaskAccumulatorLock.lock();
		if ( mTaskAccumulator.size() > 0 )
		{
			task = mTaskAccumulator.front();
			mTaskAccumulator.pop_front();
		}
		else
		{
			task = new HairTask(); // Dummy object, warning contains null pointer
		}
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return task;
}

void HairTaskProcessor::doBrush (HairShape::HairComponents::SelectedGuides &aSelectedGuides, const Vector3D< double > &aDx, BrushMode *aBrushMode)
{
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		aBrushMode->doBrush(aDx, *it);
	}
}

void HairTaskProcessor::enforceConstraints (HairShape::HairComponents::SelectedGuides &aSelectedGuides)
{
	//TODO
}

} // namespace Toolbox

} // namespace Stubble