#ifndef STUBBLE_SCALE_BRUSH_MODE_HPP
#define STUBBLE_SCALE_BRUSH_MODE_HPP

#include <maya/MVector.h>
#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the scale brush transformations.
///----------------------------------------------------------------------------------------------------
class ScaleBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush( MVector aDX, float aDT );
};

} // namespace Toolbox

} // namespace Stubble

#endif