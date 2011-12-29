#ifndef STUBBLE_ROTATE_BRUSH_MODE_HPP
#define STUBBLE_ROTATE_BRUSH_MODE_HPP

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnCamera.h>
#include "../BrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

///----------------------------------------------------------------------------------------------------
/// Class implementing the rotate brush transformation, i.e. selected hair vertices rotate around
/// the axis perpendicular to the projection plane going through the initial mouse position (position
/// at which onPress event occurred).
///----------------------------------------------------------------------------------------------------
class RotateBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// Calculates and applies rotation around an arbitrary axis for each vertex inside the brush. Takes
	/// into account brush falloff. The axis of rotation is perpendicular to the projection plane and goes
	/// through the center of the brush.
	///
	/// \param aTask Hair task object containing transformation details, see HairTask.hpp
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:

	///----------------------------------------------------------------------------------------------------
	/// Calculates the rotation transformation matrix along the given axis,
	///
	/// \param aMeasure Measure of the rotation (typically movement of the mouse along the X axis
	/// \param aAxis Axis of rotation
	///
	/// \return Resulting rotation transformation
	///----------------------------------------------------------------------------------------------------
	Matrix< Real > getRotationMatrix ( Real aMeasure, const Vector3D< Real > &aAxis );

	///----------------------------------------------------------------------------------------------------
	/// Returns the rotation axis and position (in world coordinates) from a given viewport information.
	///
	/// \param aX Cursor x coordinate
	/// \param aY Cursor y coordinate
	/// \param aView Viewport information
	/// \param[out] aAxis Axis of rotation
	/// \param[out] aPosition Position of the camera (and the rotation)
	///----------------------------------------------------------------------------------------------------
	void getRotationAxis ( short aX, short aY, M3dView &aView, Vector3D< Real > &aAxis, Vector3D< Real>  &aPosition );

	///----------------------------------------------------------------------------------------------------
	/// Calculates world coordinates translation matrices to be applied during arbitrary rotation.
	///
	/// \param aPosition Guide position (hair root)
	/// \param[out] aToOrigin Translation matrix to the origin
	/// \param[out] aFromOrigin Inverse translation
	///----------------------------------------------------------------------------------------------------
	void getTranslationMatrices ( const Vector3D< Real > &aPosition, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin);

	static const Real ROTATION_SENSITIVITY; ///< To damp too sensitive brush
};

} // namespace Toolbox

} // namespace Stubble

#endif
