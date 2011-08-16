#include "SegmentsStorage.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

const Real HAIR_LENGTH = 1.0f;

SegmentsStorage::SegmentsStorage( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// Prepare size
	mCurrent.mSegments.resize( aCurrentPositions.size() );
	// For every guide
	GuidesSegments::iterator guideIt = mCurrent.mSegments.begin();
	for ( GuidesCurrentPositions::const_iterator posIt = aCurrentPositions.begin(); 
		posIt != aCurrentPositions.end(); ++posIt, ++guideIt )
	{
		// Get segments count
		guideIt->resize( aInterpolationGroups.getSegmentsCount( posIt->mPosition.getUCoordinate(), 
			posIt->mPosition.getVCoordinate() ) );
		// For every segment
		Vector3D< Real > segmentPos;
		Vector3D< Real > segmentSize( 0, 0, HAIR_LENGTH / guideIt->size() );
		for ( Segments::iterator segIt = guideIt->begin(); segIt != guideIt->end(); ++segIt )
		{
			*segIt = segmentPos;
			segmentPos += segmentSize; // Next segment position
		}

	}
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
	const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TODO */
}

void SegmentsStorage::setFrame( Time aTime )
{
	if ( !imported() )
	{
		return; // Ignores time
	}
	/* TODO REQUIRES MATH*/
}

void SegmentsStorage::replace( const PartialStorage & aSegmentsChange )
{
	if ( imported() )
	{
		// For every time
		AllFramesSegments::iterator destIt = mSegments.begin();
		AllFramesSegments::const_iterator sourceIt = aSegmentsChange.mFrames.begin();
		for ( ; destIt != mSegments.end(); ++destIt, ++sourceIt )
		{
			// For every change
			for ( GuidesIds::const_iterator idIt = aSegmentsChange.mIds.begin(); 
				idIt != aSegmentsChange.mIds.end(); ++idIt )
			{
				// Copy segments
				destIt->second.mSegments[ *idIt ] = sourceIt->second.mSegments[ *idIt ]; 
			}
		}
		// Reset current frame
		setFrame( mCurrent.mFrame );
	}
	else
	{
		const GuidesSegments & change = aSegmentsChange.mFrames.begin()->second.mSegments;
		// For every change
		for ( GuidesIds::const_iterator idIt = aSegmentsChange.mIds.begin(); 
			idIt != aSegmentsChange.mIds.end(); ++idIt )
		{
			// Copy segments
			mCurrent.mSegments[ *idIt ] = change[ *idIt ]; 
		}
	}
}

PartialStorage * SegmentsStorage::propagateChanges( const SelectedGuides & aSelectedGuides )
{
	/* TODO REQUIRES MATH*/
	return 0;
}

void SegmentsStorage::setSegmentsCount( const GuidesCurrentPositions & aCurrentPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TODO REQUIRES MATH*/
}
	
} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble