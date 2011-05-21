#ifndef STUBBLE_BRUSH_MODE_HPP
#define STUBBLE_BRUSH_MODE_HPP

#include <maya/MVector.h>

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The interface for all of the brush mode classes.
///----------------------------------------------------------------------------------------------------
class BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method in which the derived classes implement the different brushing transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush( MVector aDX, float aDT ) = 0;
};

} // namespace Toolbox

} // namespace Stubble

#endif