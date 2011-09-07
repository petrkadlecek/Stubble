#include "GenericTool.hpp"
#include "../ToolShapes/CircleToolShape/CircleToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

GenericTool::GenericTool() :
	mShape(0)
{
}

GenericTool::GenericTool(ToolShape *aToolShape) :
	mShape(aToolShape)
{
}

M3dView* GenericTool::getActiveView()
{
	return &mView;
}

void GenericTool::drawToolShape( short aScreenCoords[ 2 ], QEvent::Type aEventType )
{
	mShape->draw( &mView, aScreenCoords, aEventType );
}

ToolShape *GenericTool::getToolShape()
{
	return mShape;
}

} // namespace Toolbox

} // namespace Stubble