#ifndef STUBBLE_SPHERE_TOOL_SHAPE_HPP
#define STUBBLE_SPHERE_TOOL_SHAPE_HPP

#include "../../Tools/GenericTool.hpp"
#include "../../Tools/HapticSettingsTool.hpp"
#include <maya/MVector.h>
//#include "../ToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Sphere brush shape.
///----------------------------------------------------------------------------------------------------
class SphereToolShape :
	public ToolShape
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
	SphereToolShape();

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
	/// Returns tool shape name.
	///
	/// \return The name of the tool shape
	///----------------------------------------------------------------------------------------------------
	virtual MString getName();

	///----------------------------------------------------------------------------------------------------
	/// Returns Sphere radius of the current tool.
	/// 
	/// \return Sphere tool shape radius
	///----------------------------------------------------------------------------------------------------
	inline int getRadius() const;

	///----------------------------------------------------------------------------------------------------
	/// Returns Sphere radius of the current tool.
	/// 
	/// \return Sphere tool shape radius
	///----------------------------------------------------------------------------------------------------
	void getPosition(MVector &aProxyPositon) const;

protected:

	bool mIsDrawn; ///< Flag signaling if there's a Sphere that needs to be erased

	MVector mPrevProxyPosition; ///< The previous position of the proxy during moving

	static const double DEFAULT_RADIUS; ///< The scale multiplier for calculating the Sphere radius.

	double mRadius; ///< The radius of the displayed Sphere (mRadius = mScale * mScaleFactor).
};

inline int SphereToolShape::getRadius() const
{
	return mRadius;
}
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_SPHERE_TOOL_SHAPE_HPP
