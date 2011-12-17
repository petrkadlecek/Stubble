#include "TranslateBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

void TranslateBrushMode::doBrush( HairTask *aTask )
{
	Vector3D< double > wdX = transformDxToWorld(aTask->mDx, aTask->mView);

	// Loop through all guides
	Vector3D< double > dX;
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias

		if (guide->mGuideSegments.mSegmentLength <= EPSILON)
		{
			continue;
		}

		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();
		const Real SCALE_FACTOR = guide->mGuideSegments.mSegmentLength; // Scale factor to be applied, so the brush behaves equally on all scales
		dX = Vector3D< double >::transform(wdX, guide->mPosition.mLocalTransformMatrix) * SCALE_FACTOR;

		// Loop through all guide segments except the first one (that's a follicle and should not move)
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if ( guide->mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				hairVertices[ i ] += (mEnableFalloff == true) ? dX * guide->mSegmentsAdditionalInfo[ i ].mFallOff : dX;
			}
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

Vector3D< double > TranslateBrushMode::transformDxToWorld( const Vector3D< double > &aDx, M3dView &aView )
{
	// Obtain the camera information
	MDagPath cameraPath;
	aView.getCamera(cameraPath);
	MFnCamera camera(cameraPath);

	// Transform the dX vector to the world space
	MStatus status;
	MVector right = camera.rightDirection(MSpace::kWorld, &status);
	MVector up = camera.upDirection(MSpace::kWorld, &status);
	MVector view = camera.viewDirection(MSpace::kWorld, &status);
	return aDx.x * right + aDx.y * up + aDx.z * view;
}

} // namespace Toolbox

} // namespace Stubble
