#include "UndoStack.hpp"

#include "HairShape\UserInterface\HairShape.hpp"

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

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