#ifndef STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP
#define STUBBLE_PUFF_ROOT_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the puff root brush transformations.
///----------------------------------------------------------------------------------------------------
class PuffRootBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( const Vector3D< double > &aDX );
};

} // namespace Toolbox

} // namespace Stubble

#endif