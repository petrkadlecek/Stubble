#ifndef STUBBLE_TOOL_SHAPE_HPP
#define STUBBLE_TOOL_SHAPE_HPP

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Circle brush shape.
///----------------------------------------------------------------------------------------------------
class CircleToolShape
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

#endif // STUBBLE_TOOL_SHAPE_HPP