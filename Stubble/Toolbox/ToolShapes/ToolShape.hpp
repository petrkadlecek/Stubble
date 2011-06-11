#ifndef STUBBLE_TOOL_SHAPE_HPP
#define STUBBLE_TOOL_SHAPE_HPP

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Abstract base class that provides brush shape.
///----------------------------------------------------------------------------------------------------
class ToolShape
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Returns shape size.
	///----------------------------------------------------------------------------------------------------
	virtual float getSize();

	/* TODO once we have GuideHair...
	vector< GuideHair > filterSelection() = 0;
	*/

	///----------------------------------------------------------------------------------------------------
	/// Draws the shape.
	///----------------------------------------------------------------------------------------------------
	virtual void draw() = 0;
protected:
	float mSize; ///< Tool shape size.
};
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_TOOL_SHAPE_HPP