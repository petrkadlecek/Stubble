#include "SegmentsStorage.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

const Real HAIR_LENGTH = 1.0f;
const unsigned int INTERPOLATE_FROM = 3;

SegmentsStorage::SegmentsStorage( const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	// Prepare size
	mCurrent.mSegments.resize( aRestPositions.size() );
	// For every guide
	GuidesSegments::iterator guideIt = mCurrent.mSegments.begin();
	for ( GuidesRestPositions::const_iterator posIt = aRestPositions.begin(); 
		posIt != aRestPositions.end(); ++posIt, ++guideIt )
	{
		// Get segments count
		guideIt->mSegments.resize( aInterpolationGroups.getSegmentsCount( posIt->mPosition.getUCoordinate(), 
			posIt->mPosition.getVCoordinate() ) );
		// For every segment
		Vector3D< Real > segmentPos;
		Vector3D< Real > segmentSize( 0, 0, HAIR_LENGTH / guideIt->mSegments.size() );
		for ( Segments::iterator segIt = guideIt->mSegments.begin(); segIt != guideIt->mSegments.end(); ++segIt )
		{
			*segIt = segmentPos;
			segmentPos += segmentSize; // Next segment position
		}
		// Set segment length
		guideIt->mSegmentLength = segmentSize.z;
	}
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
	const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	if ( aOldStorage.imported() )
	{
		// For every frame
		for ( AllFramesSegments::const_iterator it = aOldStorage.mSegments.begin(); 
			it != aOldStorage.mSegments.end(); ++it )
		{
			InterpolateFrame( it->second, aOldRestPositionsUG, aRestPositions, aInterpolationGroups, 
				// Create output frame
				mSegments.insert( std::make_pair( it->first, FrameSegments() ) ).first->second );
		}
		// Reset current frame
		setFrame( mCurrent.mFrame );
	}
	else
	{
		InterpolateFrame( aOldStorage.mCurrent, aOldRestPositionsUG, aRestPositions, aInterpolationGroups, mCurrent );
	}
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
	if ( imported() )
	{
		/* TODO REQUIRES MATH*/
		return 0;			
	}
	else
	{
		// Prepare partial storage
		PartialStorage * tmpStorage = new PartialStorage;
		// Prepare frame
		tmpStorage->mFrames.insert( std::make_pair( mCurrent.mFrame, FrameSegments() ) );
		FrameSegments & frame = tmpStorage->mFrames.begin()->second;
		// For every selected guide
		for( SelectedGuides::const_iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
		{
			if ( it->mDirtyFlag )
			{
				tmpStorage->mIds.push_back( it->mGuideId ); // Store guide id
				frame.mSegments.push_back( mCurrent.mSegments[ it->mGuideId ] ); // Store old guide segments
				mCurrent.mSegments[ it->mGuideId ] = it->mSegments; // Copy modified guide segments
			}
		}
		return tmpStorage;
	}
}

void SegmentsStorage::setSegmentsCount( const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	/* TODO REQUIRES MATH*/
}

void SegmentsStorage::InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		FrameSegments & aOutputSegments ) const
{
	// Prepare size
	aOutputSegments.mSegments.resize( aRestPositions.size() );
	// Copy frame time
	aOutputSegments.mFrame = aOldSegments.mFrame;
	// For every guide
	GuidesSegments::iterator guideIt = aOutputSegments.mSegments.begin();
	for ( GuidesRestPositions::const_iterator posIt = aRestPositions.begin(); 
	posIt != aRestPositions.end(); ++posIt, ++guideIt )
	{
		// Get interpolation group
		unsigned int interpolationGroup = aInterpolationGroups.getGroupId( posIt->mPosition.getUCoordinate(),
																			posIt->mPosition.getVCoordinate() );
		// Get segments count
		guideIt->mSegments.resize( aInterpolationGroups.getGroupSegmentsCount( interpolationGroup ) ); 
		// Now selected closest guides
		ClosestGuidesIds guidesIds;
		aOldRestPositionsUG.getNClosestGuides( posIt->mPosition.getPosition(), 
			interpolationGroup, INTERPOLATE_FROM, guidesIds );
		// First calculate cumulated distance
		Real cumulatedDistance = 0;
		for ( ClosestGuidesIds::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
		{
			cumulatedDistance += guideIdIt->mDistance;
		}
		Real inverseCumulatedDistance = 1.0f / cumulatedDistance; 
		// For every old guide segments to interpolate from
		for ( ClosestGuidesIds::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
		{
			// Old segments iterator
			Segments::const_iterator oldSegIt = aOldSegments.mSegments[ guideIdIt->mGuideId ].mSegments.begin();
			Real weight = guideIdIt->mDistance * inverseCumulatedDistance; // Simple weight TODO make better
			// For every segment
			for ( Segments::iterator segIt = guideIt->mSegments.begin(); segIt != guideIt->mSegments.end(); 
				++segIt, ++oldSegIt )
			{
				*segIt += *oldSegIt * weight;
			}
		}
		// Calculate segment length from first segment
		guideIt->mSegmentLength = Vector3D< Real >( *guideIt->mSegments.begin(), *( guideIt->mSegments.begin() + 1 ) ).size(); 
	}
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble