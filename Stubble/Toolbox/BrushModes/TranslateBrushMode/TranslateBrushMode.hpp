#ifndef STUBBLE_TRANSLATE_BRUSH_MODE_HPP
#define STUBBLE_TRANSLATE_BRUSH_MODE_HPP

#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// The class implementing the translate brush transformations.
///----------------------------------------------------------------------------------------------------
class TranslateBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush( Vector3D< double > aDX );
};

} // namespace Toolbox

} // namespace Stubble

#endif