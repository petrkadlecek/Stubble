#include "PuffRootBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

void PuffRootBrushMode::doBrush ( HairTask *aTask )
{
	// Loop through all guides
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
		const Real START_BOOST = SEGMENT_COUNT / 2;

		// Loop through all guide segments except the first one
		Real segmentLength = guide->mGuideSegments.mSegmentLength;
		Real boost = START_BOOST;
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
			boost = START_BOOST / ((i + 1) * (i + 1));
			d *= boost;
			hairVertices[ i ] += d;
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

} // namespace Toolbox

} // namespace Stubble
