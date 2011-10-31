#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MFnCamera.h>
#include "TranslateBrushMode.hpp"

namespace Stubble
{

namespace Toolbox
{

Vector3D< double > TranslateBrushMode::preBrushTransform( const Vector3D< double > &aDX, void *aTransformInfo)
{
	// Obtain the camera information from the supplied transform info
	MDagPath cameraPath;
	M3dView *view = (M3dView *)(aTransformInfo);
	view->getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// Transform the aDX to the world space
	MStatus status;
	MVector right = camera.rightDirection(MSpace::kWorld, &status);
	MVector up = camera.upDirection(MSpace::kWorld, &status);

	return Vector3D< double > (aDX.x * right + aDX.y * up);
}

void TranslateBrushMode::doBrush ( const Vector3D< double > &aDX, HairShape::HairComponents::SelectedGuide &aGuideHair )
{
	Vector3D< double > dX = Vector3D< double >::transform(aDX, aGuideHair.mPosition.mLocalTransformMatrix);
	const size_t SEGMENT_COUNT = aGuideHair.mGuideSegments.mSegments.size();

	// Loop through all guide segments except the first one (that's a follicle and should not move)
	for (size_t i = 1; i < SEGMENT_COUNT; ++i)
	{
		if ( aGuideHair.mSegmentsAdditionalInfo[ i ].mInsideBrush )
		{
			aGuideHair.mGuideSegments.mSegments[ i ] += (mEnableFalloff == true) ? dX * aGuideHair.mSegmentsAdditionalInfo[ i ].mFallOff : dX;
		}
	}
}

} // namespace Toolbox

} // namespace Stubble