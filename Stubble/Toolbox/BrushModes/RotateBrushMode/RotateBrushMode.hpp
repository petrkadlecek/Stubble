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
/// The class implementing the rotate brush transformations.
///----------------------------------------------------------------------------------------------------
class RotateBrushMode :
	public BrushMode
{
public:
	///----------------------------------------------------------------------------------------------------
	/// The method which implements the actual brush transformations.
	///----------------------------------------------------------------------------------------------------
	virtual void doBrush ( HairTask *aTask );

private:
	///----------------------------------------------------------------------------------------------------
	/// This method calculates the rotation transformation matrix along the given axis,
	///
	/// \param aMeasure	Measure of the rotation (typically movement of the mouse along the X axis
	/// \param aAxis	Axis of rotation
	/// \return			Resulting rotation transformation
	///----------------------------------------------------------------------------------------------------
	Matrix< Real > getRotationMatrix ( Real aMeasure, const Vector3D< Real > &aAxis );

	///----------------------------------------------------------------------------------------------------
	/// This method returns the rotation axis and position (in world coordinates) from a given viewport
	/// information.
	///
	/// \param aView		Viewport information
	/// \param aAxis		Out - axis of rotation
	/// \param aPosition	Out - position of the camera (and the rotation)
	///----------------------------------------------------------------------------------------------------
	void getRotationAxis ( M3dView &aView, Vector3D< Real > &aAxis, Vector3D< Real>  &aPosition );

	///----------------------------------------------------------------------------------------------------
	/// This method calculates world coordinates translation matrices to be applied during arbitrary
	/// rotation.
	///
	/// \param aPosition	Guide position (hair root)
	/// \param aToOrigin	Out - translation matrix to the origin
	/// \param aFromOrigin	Out - inverse translation
	///----------------------------------------------------------------------------------------------------
	void getTranslationMatrices ( const Vector3D< Real > &aPosition, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin);
};

} // namespace Toolbox

} // namespace Stubble

#endif