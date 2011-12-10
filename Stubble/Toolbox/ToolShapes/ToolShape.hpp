#ifndef STUBBLE_TOOL_SHAPE_HPP
#define STUBBLE_TOOL_SHAPE_HPP

#include <maya/M3dView.h>
#include <QtGui/QMouseEvent>

#include "Common/CommonTypes.hpp"

namespace Stubble
{

namespace Toolbox
{
// avoiding circular references
class GenericTool;

///----------------------------------------------------------------------------------------------------
/// Abstract base class that provides brush shape.
///----------------------------------------------------------------------------------------------------
class ToolShape
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Draws the tool shape.
	///
	/// \param aView Viewport information
	/// \param aScreenCoords Screen coordinates [x, y]
	/// \param aEventType Info about the event that triggered draw
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType ) = 0;

	///----------------------------------------------------------------------------------------------------
	/// Collects relevant modified information from the calling tool object. Called whenever something in
	/// the UI changes.
	///
	/// \param aTool Tool shape owner
	///----------------------------------------------------------------------------------------------------
	virtual void update( GenericTool *aTool ) = 0;

	///----------------------------------------------------------------------------------------------------
	/// Returns tool shape scale.
	///
	/// \return The scale (size) of the tool shape
	///----------------------------------------------------------------------------------------------------
	inline Real getScale();

protected:

	Real mScale; ///< The scale of the tool (passed from the UI).
};

inline Real ToolShape::getScale()
{
	return mScale;
}
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_TOOL_SHAPE_HPP
