#include "ScaleBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

void ScaleBrushMode::doBrush ( HairTask *aTask )
{
	// Loop through all guides
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();
		guide->mGuideSegments.mSegmentLength = getNewSegmentLength(aTask->mDx.x, guide->mGuideSegments.mSegmentLength);

		// Loop through all guide segments except the first one
		Vector3D< double > previousVertex = hairVertices[ 0 ];
		Vector3D< double > v;
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			v = hairVertices[ i ] - previousVertex;
			v.normalize();
			previousVertex = hairVertices[ i ];
			hairVertices[ i ] = hairVertices[ i - 1 ] + v * guide->mGuideSegments.mSegmentLength;
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

} // namespace Toolbox

} // namespace Stubble