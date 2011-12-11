#include "UndoStack.hpp"

#include "HairShape\UserInterface\HairShape.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

void UndoStack::add( PartialStorage * aChange )
{
	// First remove all redo steps
	for ( SegmentsChangesStackPointer p = mCurrent; p != mChanges.end(); ++p )
		delete * p;
	mChanges.erase( mCurrent, mChanges.end() );
	// If stack is too big, erase first element
	if ( mChanges.size() > MAX_STACK_SIZE )
	{
		delete *mChanges.begin();
		mChanges.pop_front();
	}
	// Add change to stack
	mChanges.push_back( aChange );
	// Finally set pointer to stack end
	mCurrent = mChanges.end();
	incrementOp();
}

void UndoStack::incrementOp()
{
	HairShape *hairShape = 0;
	if ( ( hairShape = HairShape::getActiveObject() ) != 0 )
	{	
		MObject thisNode = hairShape->asMObject();	
		MPlug dataPlug( thisNode, HairShape::operationCountAttr );
		dataPlug.setInt( dataPlug.asInt() + 1 );
	}	
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble