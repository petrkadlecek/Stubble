#include "PuffEndBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

void PuffEndBrushMode::doBrush ( HairTask *aTask )
{
	// Loop through all guides
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();

		// Loop through all guide segments except the first one
		Real segmentLength = guide->mGuideSegments.mSegmentLength;
		Vector3D< Real > vertexAtNormal;
		Vector3D< Real > distance;
		Vector3D< Real > d;
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if ( !guide->mSegmentsAdditionalInfo[ i ].mInsideBrush )
			{
				continue;
			}
			vertexAtNormal.set(0.0, 0.0, i * segmentLength);
			distance = vertexAtNormal - hairVertices[ i ];
			d = (mEnableFalloff == true) ? distance * aTask->mDx.x * guide->mSegmentsAdditionalInfo[ i ].mFallOff : distance * aTask->mDx.x;
			hairVertices[ i ] += d;
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

} // namespace Toolbox

} // namespace Stubble