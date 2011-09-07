#ifndef STUBBLE_TOOL_SHAPE_HPP
#define STUBBLE_TOOL_SHAPE_HPP

#include <maya/M3dView.h>
#include <QtGui/QMouseEvent>

//#include "../Tools/GenericTool.hpp"

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
	/// Returns shape size.
	///----------------------------------------------------------------------------------------------------
	//Deprecated - TODO: probably remove?
	//virtual float getSize();

	// TODO: for Honza Kadlec: feel free to add/remove any other methods/members. For the time being I've
	// commented out method for getting size of the shape. This should go to inherited shapes.

	/* TODO once we have GuideHair...
	vector< GuideHair > filterSelection() = 0;
	*/

	///----------------------------------------------------------------------------------------------------
	/// Draws the tool shape.
	///----------------------------------------------------------------------------------------------------
	virtual void draw( M3dView *aView, short aScreenCoords[ 2 ], QEvent::Type aEventType ) = 0;

	///----------------------------------------------------------------------------------------------------
	/// Collects relevant modified information from the calling tool object.
	///----------------------------------------------------------------------------------------------------
	virtual void update( GenericTool *aTool ) = 0;

protected:
	//Deprecated - TODO: probably remove?
	//float mSize; ///< Tool shape size.
};
	
} // namespace Toolbox

} // namespace Stubble

#endif // STUBBLE_TOOL_SHAPE_HPP