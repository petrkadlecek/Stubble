#include <limits>
#include "ScaleBrushMode.hpp"
#include "Toolbox/HairTask.hpp"

namespace Stubble
{

namespace Toolbox
{

typedef std::numeric_limits< Real > RealLim;

void ScaleBrushMode::doBrush ( HairTask *aTask )
{
	// Loop through all guides
	HairShape::HairComponents::SelectedGuides::iterator it;
	for (it = aTask->mAffectedGuides->begin(); it != aTask->mAffectedGuides->end(); ++it)
	{
		HairShape::HairComponents::SelectedGuide *guide = *it; // Guide alias
		HairShape::HairComponents::Segments &hairVertices = guide->mGuideSegments.mSegments; // Local alias
		const size_t SEGMENT_COUNT = hairVertices.size();
		Real newLength = getNewSegmentLength(aTask->mDx.x, guide->mGuideSegments.mSegmentLength);
		const Real SCALE_FACTOR = newLength / guide->mGuideSegments.mSegmentLength;
		guide->mGuideSegments.mSegmentLength = newLength;

		hairVertices[ 0 ].set(0.0, 0.0, 0.0);
		// Loop through all guide segments except the first one
		for (size_t i = 1; i < SEGMENT_COUNT; ++i)
		{
			if (SCALE_FACTOR == RealLim::infinity() || SCALE_FACTOR <= 0.0)
			{
				hairVertices[ i ].set(0.0, 0.0, i * newLength);
			}
			else
			{
				hairVertices[ i ] *= SCALE_FACTOR;
			}
		}

		guide->mDirtyFlag = true;
		guide->mDirtyRedrawFlag = true;
	}
}

} // namespace Toolbox

} // namespace Stubble