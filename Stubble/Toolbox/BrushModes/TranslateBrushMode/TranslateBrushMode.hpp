#ifndef STUBBLE_TRANSLATE_BRUSH_MODE_HPP
#define STUBBLE_TRANSLATE_BRUSH_MODE_HPP

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnCamera.h>
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
	virtual void doBrush ( HairTask *aTask );

private:
	///----------------------------------------------------------------------------------------------------
	/// Transforms the dX vector into the world coordinates.
	///
	/// \param aDx		The vector to be transformed
	/// \param aView	Viewport information
	/// \return			Vector in the world coordinates
	///----------------------------------------------------------------------------------------------------
	Vector3D< double > transformDxToWorld( const Vector3D< double > &aDx, M3dView &aView );
};

} // namespace Toolbox

} // namespace Stubble

#endif