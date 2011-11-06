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
	/// This method calculates the rotation transformation along the axis perpendicular to the screen
	/// going through the middle of the brush.
	///
	/// \param aMeasure	Measure of the rotation (typically movement of the mouse along the X axis
	/// \param aView	Viewport information
	/// \return			World coordinate rotation transformation
	///----------------------------------------------------------------------------------------------------
	Matrix< Real > getRotationMatrix ( Real aMeasure, M3dView &aView );

	///----------------------------------------------------------------------------------------------------
	/// This method returns the rotation axis from a given viewport informatin.
	///
	/// \param aView	Viewport information
	/// \return			World coordinate rotation axis
	///----------------------------------------------------------------------------------------------------
	Vector3D< Real > getRotationAxis ( M3dView &aView );

	///----------------------------------------------------------------------------------------------------
	/// This method calculates world coordinates translation matrices to be applied during arbitrary
	/// rotation - moving the rotation matrix from the camera position to the origin and back
	///
	/// \param aView		Viewport information
	/// \param aToOrigin	Out - translation matrix to the origin
	/// \param aFromOrigin	Out - inverse translation
	///----------------------------------------------------------------------------------------------------
	void getTranslationMatrices ( M3dView &aView, Matrix< Real > &aToOrigin, Matrix< Real > &aFromOrigin);
};

} // namespace Toolbox

} // namespace Stubble

#endif