#include "SegmentsStorage.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

SegmentsStorage::SegmentsStorage( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
	const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}

void SegmentsStorage::setFrame( Time aTime )
{
}

void SegmentsStorage::replace( const PartialStorage & aSegmentsChange )
{
}

PartialStorage * SegmentsStorage::propagateChanges( const SelectedGuides & aSelectedGuides )
{
	/* TODO */
	return 0;
}

void SegmentsStorage::importSegments( const FrameSegments & aFrameSegments )
{
}

void SegmentsStorage::setSegmentsCount( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}
	
} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble