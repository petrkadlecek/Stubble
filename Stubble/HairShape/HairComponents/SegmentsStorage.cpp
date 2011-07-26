#include "SegmentsStorage.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

SegmentsStorage::SegmentsStorage( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, const HairLength::LengthInfo & aLengthInfo )
{
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
	const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, const HairLength::LengthInfo & aLengthInfo )
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

void SegmentsStorage::setLength( const GuidesCurrentPositions & aCurrentPositions, const HairLength::LengthInfo & aLengthInfo )
{
}

void SegmentsStorage::setSegmentsCount( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
}
	
} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble