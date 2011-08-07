#ifndef STUBBLE_BRUSH_MODE_HPP
#define STUBBLE_BRUSH_MODE_HPP

#include "../../Primitives/Vector3D.hpp"

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
	///
	/// \param aDX Cursor change in local coordinates
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush( Vector3D< double > aDX ) = 0;
};

} // namespace Toolbox

} // namespace Stubble

#endif