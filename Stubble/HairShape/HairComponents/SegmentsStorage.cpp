#include "SegmentsStorage.hpp"

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

const Real HAIR_LENGTH = 1.0f;
const unsigned __int32 INTERPOLATE_FROM = 3;

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

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const GuidesIds & aRemainingGuides )
{
	if ( aOldStorage.imported() )
	{
		// For every frame
		for ( AllFramesSegments::const_iterator it = aOldStorage.mSegments.begin(); 
			it != aOldStorage.mSegments.end(); ++it )
		{
			FrameSegments & aOutputSegments = mSegments.insert( std::make_pair( it->first, FrameSegments() ) ).first->second;
			aOutputSegments.mFrame = it->first;
			aOutputSegments.mSegments.resize( aRemainingGuides.size() );
			GuidesIds::const_iterator idIt = aRemainingGuides.begin();
			// For every guide
			for ( GuidesSegments::iterator guideIt = aOutputSegments.mSegments.begin(); 
				guideIt != aOutputSegments.mSegments.end(); ++guideIt, ++ idIt )
			{
				*guideIt = it->second.mSegments[ *idIt ]; // Copy guide segments
			}
		}
		// Reset current frame
		setFrame( mCurrent.mFrame );
	}
	else
	{
		mCurrent.mSegments.resize( aRemainingGuides.size() );
		GuidesIds::const_iterator idIt = aRemainingGuides.begin();
		// For every guide
		for ( GuidesSegments::iterator guideIt = mCurrent.mSegments.begin(); 
			guideIt != mCurrent.mSegments.end(); ++guideIt, ++ idIt )
		{
			*guideIt = aOldStorage.mCurrent.mSegments[ *idIt ]; // Copy guide segments
		}
	}
}

void SegmentsStorage::setFrame( Time aTime )
{
	if ( !imported() )
	{
		return; // Ignores time
	}
	/* TODO MAY BE SOMETHING BETTER THAN LINEAR INTERPOLATION*/
	/* TODO PROPAGATE CHANGES */
	mCurrent.mFrame = aTime;
	// Get bounds first
	AllFramesSegments::const_iterator lowerBound = mSegments.lower_bound( aTime );
	// Lower bound point to first element, which is equal or larger
	// 1.we found equal element or all elements are greater -> no interpolation
	if ( lowerBound->first == aTime || lowerBound == mSegments.begin() )
	{
		mCurrent.mSegments = lowerBound->second.mSegments;
	}
	else
	{
		if ( lowerBound == mSegments.end() ) // 2.Nothing is larger or equal -> no interpolation
		{
			mCurrent.mSegments = (--lowerBound)->second.mSegments;
		}
		else
		{
			// No we now for sure, that there is at least one lesser element and one greater element
			// lowerBound now points to first greater element
			// --lowerBound will point to last lesser element
			GuidesSegments::const_iterator upIt = lowerBound->second.mSegments.begin();
			Real upFactor = static_cast< Real >( lowerBound->first - aTime );
			GuidesSegments::const_iterator lowIt = (--lowerBound)->second.mSegments.begin();
			Real lowFactor = static_cast< Real >( aTime - lowerBound->first );
			Real inverseTotalFactor = 1.0f / ( upFactor + lowFactor );
			upFactor *= inverseTotalFactor;
			lowFactor *= inverseTotalFactor;
			// Finaly we can begin to copy
			mCurrent.mSegments.resize( lowerBound->second.mSegments.size() );
			// For every guide
			for ( GuidesSegments::iterator guideIt = mCurrent.mSegments.begin(); 
				guideIt != mCurrent.mSegments.end(); ++guideIt, ++upIt, ++lowIt )
			{
				// For every segment
				Segments::const_iterator upSegIt = upIt->mSegments.begin();	
				Segments::const_iterator lowSegIt = lowIt->mSegments.begin();
				for ( Segments::iterator segIt = guideIt->mSegments.begin(); 
					segIt != guideIt->mSegments.end(); ++segIt, ++upSegIt, ++lowSegIt )
				{
					// Interpolate
					*segIt = *upSegIt * upFactor + *lowSegIt * lowFactor;
				}
				/* TODO RECALCULATE SEGMENTS TO HAVE SAME LENGTH */
			}

		}
	}
	AllFramesSegments::const_iterator upperBound = mSegments.upper_bound( aTime );
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
				mCurrent.mSegments[ it->mGuideId ] = it->mGuideSegments; // Copy modified guide segments
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

BoundingBox SegmentsStorage::getBoundingBox( const GuidesCurrentPositions & aCurrentPositions ) const
{
	BoundingBox bbox;
	GuidesCurrentPositions::const_iterator posIt = aCurrentPositions.begin();
	// For every guide
	for ( GuidesSegments::const_iterator guideIt = mCurrent.mSegments.begin();
		guideIt != mCurrent.mSegments.end(); ++guideIt, ++posIt )
	{
		// For every segment
		for ( Segments::const_iterator segIt = guideIt->mSegments.begin();
			segIt != guideIt->mSegments.end(); ++segIt )
		{
			// Transform to world and expand bbox
			bbox.expand( posIt->mPosition.toWorld( *segIt ) );
		}
	}
	return bbox;
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
		unsigned __int32 interpolationGroup = aInterpolationGroups.getGroupId( posIt->mPosition.getUCoordinate(),
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
		/* TODO this make create segments with different lengths in one guide */
		for ( ClosestGuidesIds::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
		{
			// Old segments iterator
			Segments::const_iterator oldSegIt = aOldSegments.mSegments[ guideIdIt->mGuideId ].mSegments.begin();
			Real weight = guideIdIt->mDistance * inverseCumulatedDistance; 
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