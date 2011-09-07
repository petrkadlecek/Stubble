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
			HairShape::HairComponents::SelectedGuides selectedGuides;
			task->mParentHairShape->getSelectedGuidesUG().select(task->mToolShape, selectedGuides);
			
			hairTaskProcessor->doBrush(selectedGuides, task->mDx, task->mBrushMode);
			hairTaskProcessor->enforceConstraints(selectedGuides);

			task->mParentHairShape->updateGuides(false);

			delete task;
		}
	}

	return 0;
}

size_t HairTaskProcessor::getTask (HairTask *aTask)
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
		else
		{
			aTask = 0;
		}
	mTaskAccumulatorLock.unlock();
	// ------------------------------------
	// End critical section
	// ------------------------------------

	return queueSize;
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
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it)
	{
		//TODO
	}
}

} // namespace Toolbox

} // namespace Stubble