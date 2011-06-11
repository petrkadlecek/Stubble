#ifndef STUBBLE_CIRCLE_TOOL_SHAPE_HPP
#define STUBBLE_CIRCLE_TOOL_SHAPE_HPP

#include "../ToolShape.hpp"

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
	/* TODO once we have GuideHair...
	vector< GuideHair > filterSelection();
	*/

	///----------------------------------------------------------------------------------------------------
	/// Draws the shape.
	///----------------------------------------------------------------------------------------------------
	virtual void draw();
};
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_CIRCLE_TOOL_SHAPE_HPP