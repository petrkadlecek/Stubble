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
	///
	/// \param	aView	Viewport information
	/// \param	aScreenCoords	Screen coordinates [x, y]
	/// \param	aEventType	Info about the event that triggered draw
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType );

	///----------------------------------------------------------------------------------------------------
	/// Manages tool shape attribute update - called whenever the user changes something in the UI
	///
	/// \param	aTool	Parent of this object.
	///----------------------------------------------------------------------------------------------------
	virtual void update( GenericTool *aTool );

	///----------------------------------------------------------------------------------------------------
	/// Returns circle radius of the current tool.
	/// 
	/// \return Tool shape radius
	///----------------------------------------------------------------------------------------------------
	inline int getRadius() const;

protected:

	///----------------------------------------------------------------------------------------------------
	/// Helper method that draws the actual tool shape.
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( M3dView *aView, short aScreenCoords[ 2 ] );

	bool mIsDrawn; ///< Is there a circle that needs to be erased

	short mPrevScreenCoords[ 2 ]; ///< The previous position of the cursor during moving

	static const double DEFAULT_RADIUS; ///< The scale multiplier for calculating the circle radius.

	double mRadius; ///< The radius of the displayed circle (mRadius = mScale * mScaleFactor).
};

inline int CircleToolShape::getRadius() const
{
	return mRadius;
}
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_CIRCLE_TOOL_SHAPE_HPP