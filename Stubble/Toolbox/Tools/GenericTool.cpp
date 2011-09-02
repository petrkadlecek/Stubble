#include "GenericTool.hpp"
#include "../ToolShapes/CircleToolShape/CircleToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

GenericTool::GenericTool()
	: mCircleRadius( 30 )
{
	mShape = new CircleToolShape();
}

M3dView* GenericTool::getActiveView()
{
	return &mView;
}

int GenericTool::getCircleRadius()
{
	return mCircleRadius;
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