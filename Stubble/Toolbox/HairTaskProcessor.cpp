#include "HairTaskProcessor.hpp"

namespace Stubble
{

namespace Toolbox
{

// ----------------------------------------------------------------------------
// Static data members and constatns:
// ----------------------------------------------------------------------------

HairTaskProcessor *HairTaskProcessor::mInstance = 0;
bool HairTaskProcessor::mIsRunning = false;
MSpinLock HairTaskProcessor::mIsRunningLock;

// ----------------------------------------------------------------------------
// Methods:
// ----------------------------------------------------------------------------

void HairTaskProcessor::waitFinishWorkerThread ()
{
	//TODO
}

void HairTaskProcessor::enqueueTask (HairTask *aTask)
{
	//TODO
}

void HairTaskProcessor::purgeAccumulator ()
{
	//TODO
}

void HairTaskProcessor::tryCreateWorkerThread ()
{
	//TODO
}

void HairTaskProcessor::workerFinishedCB (void *aData)
{
	//TODO
}

void HairTaskProcessor::asyncWorkerLoop (void *aData)
{
	//TODO
}

HairTask *HairTaskProcessor::getTask ()
{
	//TODO
	return 0;
}

void HairTaskProcessor::doBrush (HairTask *aTask)
{
	//TODO
}

void HairTaskProcessor::enforceConstraints (HairTask *aTask)
{
	//TODO
}

} // namespace Toolbox

} // namespace Stubble