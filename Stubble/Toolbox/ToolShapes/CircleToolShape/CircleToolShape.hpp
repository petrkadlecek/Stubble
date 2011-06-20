#ifndef STUBBLE_CIRCLE_TOOL_SHAPE_HPP
#define STUBBLE_CIRCLE_TOOL_SHAPE_HPP

#include "../../Tools/GenericTool.hpp"
//#include "../ToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Circle brush shape.
///----------------------------------------------------------------------------------------------------
class CircleToolShape :
	public ToolShape
{
public:

	CircleToolShape();


	/* TODO once we have GuideHair...
	vector< GuideHair > filterSelection();
	*/

	///----------------------------------------------------------------------------------------------------
	/// Manages the tool shape drawing.
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType );

	virtual void update( GenericTool *aTool );

protected:

	///----------------------------------------------------------------------------------------------------
	/// Helper method that draws the actual tool shape.
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( M3dView *aView, short aScreenCoords[ 2 ] );

	bool mIsDrawn; ///< Is there a circle that needs to be erased

	short mPrevScreenCoords[ 2 ]; ///< The previous position of the cursor during moving

	int mRadius; ///< The radius of the circle.
};
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_CIRCLE_TOOL_SHAPE_HPP