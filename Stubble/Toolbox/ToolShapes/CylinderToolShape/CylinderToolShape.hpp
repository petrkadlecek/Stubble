#ifndef STUBBLE_CYLINDER_TOOL_SHAPE_HPP
#define STUBBLE_CYLINDER_TOOL_SHAPE_HPP

#include "../../Tools/GenericTool.hpp"
#include "../../Tools/HapticSettingsTool.hpp"
#include <maya/MVector.h>
//#include "../ToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Cylinder brush shape.
///----------------------------------------------------------------------------------------------------
class CylinderToolShape :
	public ToolShape
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
	CylinderToolShape();

	///----------------------------------------------------------------------------------------------------
	/// Draws the tool shape.
	///
	/// \param aView Viewport information
	/// \param aProxyPosition proxy position [x, y, z]
	/// \param aEventType Info about the event that triggered draw
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType );

	/// TODO
	virtual void draw( M3dView *aView, MVector &aProxyPosition, MVector &aHapticProxyRotation, double &aHapticProxyRotationAngle );

	///----------------------------------------------------------------------------------------------------
	/// Collects relevant modified information from the calling tool object. Called whenever something in
	/// the UI changes.
	///
	/// \param aTool Tool shape owner
	///----------------------------------------------------------------------------------------------------
	virtual void update( GenericTool *aTool );

	///----------------------------------------------------------------------------------------------------
	/// Returns Cylinder radius of the current tool.
	/// 
	/// \return Cylinder tool shape radius
	///----------------------------------------------------------------------------------------------------
	inline int getRadius() const;

	///----------------------------------------------------------------------------------------------------
	/// Returns Cylinder radius of the current tool.
	/// 
	/// \return Cylinder tool shape radius
	///----------------------------------------------------------------------------------------------------
	void getPosition(MVector &aProxyPositon) const;

protected:

	bool mIsDrawn; ///< Flag signaling if there's a Cylinder that needs to be erased

	MVector mPrevProxyPosition; ///< The previous position of the proxy during moving

	static const double DEFAULT_RADIUS; ///< The scale multiplier for calculating the Cylinder radius.

	double mRadius; ///< The radius of the displayed Cylinder (mRadius = mScale * mScaleFactor).
};

inline int CylinderToolShape::getRadius() const
{
	return mRadius;
}
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_CYLINDER_TOOL_SHAPE_HPP
