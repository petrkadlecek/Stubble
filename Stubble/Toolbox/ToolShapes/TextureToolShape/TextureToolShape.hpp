#ifndef STUBBLE_TEXTURE_TOOL_SHAPE_HPP
#define STUBBLE_TEXTURE_TOOL_SHAPE_HPP

#include "../../Tools/GenericTool.hpp"
//#include "../ToolShape.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Circle brush shape.
///----------------------------------------------------------------------------------------------------
class TextureToolShape :
	public ToolShape
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Default constructor.
	///----------------------------------------------------------------------------------------------------
	TextureToolShape();

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
	/// Returns half side of the shape in current tool.
	/// 
	/// \return Texture tool shape half side
	///----------------------------------------------------------------------------------------------------
	inline int getHalfSide() const;

protected:

	///----------------------------------------------------------------------------------------------------
	/// Helper method that draws the actual tool shape.
	///
	/// \param aView Viewport information
	/// \param aScreenCoords Screen coordinates [x, y]
	///----------------------------------------------------------------------------------------------------
	void drawToolShape( M3dView *aView, short aScreenCoords[ 2 ] );

	bool mIsDrawn; ///< Flag signaling if there's a squere (texture) that needs to be erased

	short mPrevScreenCoords[ 2 ]; ///< The previous position of the cursor during moving

	static const double DEFAULT_HALF_SIDE; ///< The scale multiplier for calculating the circle radius.

	double mHalfSide; ///< The half side of the displayed square (mHalfSide = mScale * mScaleFactor).
};

inline int TextureToolShape::getHalfSide() const
{
	return mHalfSide;
}
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_TEXTURE_TOOL_SHAPE_HPP
