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

	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
	CircleToolShape();

	///----------------------------------------------------------------------------------------------------
	/// Draws the tool shape.
	///
	/// \param aView Viewport information
	/// \param aScreenCoords Screen coordinates [x, y]
	/// \param aEventType Info about the event that triggered draw
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType );

	// TODO
	virtual void draw( M3dView *aView, MVector &aProxyPosition, MVector &aHapticProxyRotation, double &aHapticProxyRotationAngle );

	///----------------------------------------------------------------------------------------------------
	/// Collects relevant modified information from the calling tool object. Called whenever something in
	/// the UI changes.
	///
	/// \param aTool Tool shape owner
	///----------------------------------------------------------------------------------------------------
	virtual void update( GenericTool *aTool );

	///----------------------------------------------------------------------------------------------------
	/// Returns tool shape name.
	///
	/// \return The name of the tool shape
	///----------------------------------------------------------------------------------------------------
	virtual MString getName();

	///----------------------------------------------------------------------------------------------------
	/// Returns circle radius of the current tool.
	/// 
	/// \return Circle tool shape radius
	///----------------------------------------------------------------------------------------------------
	inline int getRadius() const;

protected:

	///----------------------------------------------------------------------------------------------------
	/// Helper method that draws the actual tool shape.
	///
	/// \param aView Viewport information
	/// \param aScreenCoords Screen coordinates [x, y]
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( M3dView *aView, short aScreenCoords[ 2 ] );

	bool mIsDrawn; ///< Flag signaling if there's a circle that needs to be erased

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
