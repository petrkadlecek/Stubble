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
/// Class implementing the translate brush transformation, i.e. the brush repositions hair vertices in
/// a plane that is parallel to the projection plane.
///----------------------------------------------------------------------------------------------------
class TranslateBrushMode :
	public BrushMode
{
public:

	///----------------------------------------------------------------------------------------------------
	/// Loops through all guide hair and their segments and repositions all vertices marked as being inside
	/// the brush along the move vector supplied in the HairTask structure taking into account possible
	/// falloff. Rescales segments to the unit length for exact behaviour across varying scales.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:

	///----------------------------------------------------------------------------------------------------
	/// Transforms the dX vector into the world coordinates.
	///
	/// \param aDx The vector to be transformed
	/// \param aView Viewport information
	///
	/// \return Vector in the world coordinates
	///----------------------------------------------------------------------------------------------------
	Vector3D< double > transformDxToWorld( const Vector3D< double > &aDx, M3dView &aView );
};

} // namespace Toolbox

} // namespace Stubble

#endif
