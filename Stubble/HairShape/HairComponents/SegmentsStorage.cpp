#include "SegmentsStorage.hpp"
#include <cmath>
#include <cassert>

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{

const Real HAIR_LENGTH = 5.0f;

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
			posIt->mPosition.getVCoordinate() ) + 1 );
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
	const Interpolation::InterpolationGroups & aInterpolationGroups,
	unsigned __int32 aNumberOfGuidesToInterpolateFrom )
{
	if ( aOldStorage.imported() )
	{
		// For every frame
		for ( AllFramesSegments::const_iterator it = aOldStorage.mSegments.begin(); 
			it != aOldStorage.mSegments.end(); ++it )
		{
			InterpolateFrame( it->second, aOldRestPositionsUG, aRestPositions, aInterpolationGroups,
				aNumberOfGuidesToInterpolateFrom,
				// Create output frame
				mSegments.insert( std::make_pair( it->first, FrameSegments() ) ).first->second );
		}
	}
	// Interpolate current segments
	InterpolateFrame( aOldStorage.mCurrent, aOldRestPositionsUG, aRestPositions, aInterpolationGroups, 
		aNumberOfGuidesToInterpolateFrom, mCurrent );
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
	}
	// Import current segments
	mCurrent.mSegments.resize( aRemainingGuides.size() );
	GuidesIds::const_iterator idIt = aRemainingGuides.begin();
	// For every guide
	for ( GuidesSegments::iterator guideIt = mCurrent.mSegments.begin(); 
		guideIt != mCurrent.mSegments.end(); ++guideIt, ++ idIt )
	{
		*guideIt = aOldStorage.mCurrent.mSegments[ *idIt ]; // Copy guide segments
	}
}

void SegmentsStorage::setFrame( Time aTime )
{
	if ( !imported() )
	{
		return; // Ignores time
	}
	propagateChangesThroughTime(); // Saves current frame
	/* TODO MAY BE SOMETHING BETTER THAN LINEAR INTERPOLATION*/
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
				uniformlyRepositionSegments( *guideIt, static_cast< unsigned __int32 >( guideIt->mSegments.size() ) );
			}

		}
	}
}

PartialStorage * SegmentsStorage::replace( const PartialStorage & aSegmentsChange )
{
	// Prepare partial storage
	PartialStorage * tmpStorage = new PartialStorage;
	// Copy ids
	tmpStorage->mIds = aSegmentsChange.mIds;
	// Prepare storage size
	tmpStorage->mSegments.resize( aSegmentsChange.mSegments.size() );
	GuidesSegments::iterator storeIt = tmpStorage->mSegments.begin();
	GuidesSegments::const_iterator it = aSegmentsChange.mSegments.begin();
	// For every change
	for ( GuidesIds::const_iterator idIt = aSegmentsChange.mIds.begin(); 
		idIt != aSegmentsChange.mIds.end(); ++idIt, ++it, ++storeIt )
	{
		// Copy segments
		OneGuideSegments & guide = mCurrent.mSegments[ *idIt ];
		*storeIt = guide;
		guide = *it; 
	}
	// Current segments has been changed and need to be propagated through time
	mAreCurrentSegmentsDirty = true;
	return tmpStorage;
}

PartialStorage * SegmentsStorage::propagateChanges( const SelectedGuides & aSelectedGuides )
{
	// Prepare partial storage
	PartialStorage * tmpStorage = new PartialStorage;
	// For every selected guide
	for( SelectedGuides::const_iterator it = aSelectedGuides.begin(); it != aSelectedGuides.end(); ++it )
	{
		if ( (*it)->mDirtyFlag )
		{
			tmpStorage->mIds.push_back( (*it)->mGuideId ); // Store guide id
			tmpStorage->mSegments.push_back( mCurrent.mSegments[ (*it)->mGuideId ] ); // Store old guide segments
			mCurrent.mSegments[ (*it)->mGuideId ] = (*it)->mGuideSegments; // Copy modified guide segments
		}
	}
	// Current segments has been changed and need to be propagated through time
	mAreCurrentSegmentsDirty = true;
	return tmpStorage;
}

void SegmentsStorage::setSegmentsCount( const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups )
{
	if ( imported() )
	{
		// For every time
		for ( AllFramesSegments::iterator frmIt = mSegments.begin(); frmIt != mSegments.end(); ++frmIt )
		{
			GuidesSegments & guides = frmIt->second.mSegments;
			GuidesRestPositions::const_iterator posIt = aRestPositions.begin();
			// For every guide
			for ( GuidesSegments::iterator guideIt = guides.begin(); guideIt != guides.end(); ++guideIt, ++posIt )
			{
				uniformlyRepositionSegments( *guideIt, 
					// Load new segments count from interpolation groups object
					aInterpolationGroups.getSegmentsCount( posIt->mPosition.getUCoordinate(), 
					posIt->mPosition.getVCoordinate() ) + 1 );
			}
		}
	}
	// Set segments count for current frame
	GuidesSegments & guides = mCurrent.mSegments;
	GuidesRestPositions::const_iterator posIt = aRestPositions.begin();
	// For every guide
	for ( GuidesSegments::iterator guideIt = guides.begin(); guideIt != guides.end(); ++guideIt, ++posIt )
	{
		uniformlyRepositionSegments( *guideIt, 
			// Load new segments count from interpolation groups object
			aInterpolationGroups.getSegmentsCount( posIt->mPosition.getUCoordinate(), 
			posIt->mPosition.getVCoordinate() ) + 1 );
	}
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

void SegmentsStorage::uniformlyRepositionSegments( OneGuideSegments & aGuideSegments, unsigned __int32 aCount )
{
	unsigned __int32 currentSgmtCount = (unsigned __int32)aGuideSegments.mSegments.size();
	if ( aCount < 2 ) //TODO: O really? What do we exactly do?
	{
		throw new StubbleException("New hair segments count less than 2.");
	}

	Real currentSgmtLength = aGuideSegments.mSegmentLength;
	Real newSgmtLegth = currentSgmtLength * (Real)(currentSgmtCount - 1) / (Real)(aCount - 1);

	Segments newSegments;
	newSegments.reserve(aCount);
	// Position new segments except the last one
	for (unsigned __int32 i = 0; i < aCount - 1; ++i)
	{
		// Interpolate new segment position
		Real intPart;
		Real t = modf(i * newSgmtLegth / currentSgmtLength, &intPart);
		unsigned __int32 index = (unsigned __int32)intPart;
		assert(index + 1 < currentSgmtCount);
		Vector3D< Real > p = aGuideSegments.mSegments[ index ] + (aGuideSegments.mSegments[ index + 1 ] - aGuideSegments.mSegments[ index ]) * t;

		newSegments.push_back(p);
	}
	newSegments.push_back(aGuideSegments.mSegments[ currentSgmtCount - 1 ]);

	//TODO: enforce constraints

	// Replace data
	aGuideSegments.mSegmentLength = newSgmtLegth;
	aGuideSegments.mSegments = newSegments;
}

void SegmentsStorage::InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		unsigned __int32 aNumberOfGuidesToInterpolateFrom,
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
		guideIt->mSegments.resize( aInterpolationGroups.getGroupSegmentsCount( interpolationGroup ) + 1 ); 
		// Now selected closest guides
		ClosestGuidesIds guidesIds;
		aOldRestPositionsUG.getNClosestGuides( posIt->mPosition.getPosition(), 
			interpolationGroup, aNumberOfGuidesToInterpolateFrom, guidesIds );
		// First find max and min
		float maxDistance = sqrtf( guidesIds.begin()->mDistance );
		ClosestGuidesIds::const_iterator first = guidesIds.begin();
		for ( ClosestGuidesIds::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
		{
			// Refresh closest guide
			if ( first->mDistance > guideIdIt->mDistance )
			{
				first = guideIdIt;
			}
		}
		// First check, if they are not on the same position
		if ( first->mDistance < 0.0001f )
		{
			// Just copy
			* guideIt =  aOldSegments.mSegments[ first->mGuideId ];
		}
		else
		{
			// Bias distance with respect to farthest guide
			for ( ClosestGuidesIds::iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
			{
				float & distance = guideIdIt->mDistance;
				distance = ( maxDistance - distance ) / ( maxDistance * distance );
				distance *= distance;
			}
			// Finaly calculate cumulated distance
			Real cumulatedDistance = 0;
			for ( ClosestGuidesIds::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
			{
				cumulatedDistance += guideIdIt->mDistance;
			}
			Real inverseCumulatedDistance = 1.0 / cumulatedDistance;
			// For every old guide segments to interpolate from
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
			uniformlyRepositionSegments( *guideIt, static_cast< unsigned __int32 >( guideIt->mSegments.size() ) );
		}
	}
}

Real SegmentsStorage::timeAffectFactor( Time aTimeDifference )
{
	return static_cast< Real >( 1.0f - 0.2f * aTimeDifference ); // TODO SIMPLE : linear through 10 frames
}

void SegmentsStorage::propagateChangesThroughTime()
{
	if ( imported() && mAreCurrentSegmentsDirty )
	{
		// Gets first non-less frame 
		AllFramesSegments::iterator lowerBound = mSegments.lower_bound( mCurrent.mFrame );
		Real factor;
		// For every succeeding frame until change has no meaning
		for ( AllFramesSegments::iterator it = lowerBound; 
			it != mSegments.end() && ( factor = timeAffectFactor( it->first - mCurrent.mFrame ) ) > 0; ++it )
		{
			propageteChangesToFrame( it->second.mSegments, factor );
		}
		if ( lowerBound != mSegments.begin() ) // No predecessor
		{
			// Get reverse iterator
			AllFramesSegments::reverse_iterator rIt( lowerBound );
			// For every preceding frame until change has no meaning
			for ( ; rIt != mSegments.rend() && ( factor = timeAffectFactor( mCurrent.mFrame - rIt->first ) ) > 0; 
				++rIt )
			{
				propageteChangesToFrame( rIt->second.mSegments, factor );
			}
		}
		
	}
	mAreCurrentSegmentsDirty = false;
}

void SegmentsStorage::propageteChangesToFrame( GuidesSegments & aGuides, Real aFactor )
{
	GuidesSegments::const_iterator sourceIt = mCurrent.mSegments.begin();
	// For every guide
	for ( GuidesSegments::iterator destIt = aGuides.begin(); destIt != aGuides.end(); ++destIt, ++sourceIt )
	{
		Segments::const_iterator sourceSegIt = sourceIt->mSegments.begin();
		// For every segment
		for ( Segments::iterator destSegIt = destIt->mSegments.begin(); destSegIt != destIt->mSegments.end(); 
			++destSegIt, ++sourceSegIt )
		{
			*destSegIt = *sourceSegIt * aFactor + *destSegIt * ( 1 - aFactor );
		}
		uniformlyRepositionSegments( *destIt, static_cast< unsigned __int32 >( destIt->mSegments.size() ) );
	}
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble