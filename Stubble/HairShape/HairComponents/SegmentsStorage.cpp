#include "Toolbox\HairTaskProcessor.hpp"
#include "SegmentsStorage.hpp"
#include <cmath>
#include <cassert>

namespace Stubble
{

namespace HairShape
{
	
namespace HairComponents
{


const Real FRAME_PROPAGATION_TO_FRAMES_INVERSE = 0.05f;	///< Too how many frames should one frame propagate INVERSED

SegmentsStorage::SegmentsStorage( const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups, Real aLength ):
	mStartLength ( aLength )
{
	// Prepare size
	mCurrent.mSegments.resize( aRestPositions.size() );
	// For every guide
	#ifdef _OPENMP
    #pragma omp parallel for schedule( guided )
    #endif
	for ( int i = 0; i < static_cast< int >( aRestPositions.size() ); ++i )
	{
		const GuideRestPosition & pos = aRestPositions[ i ];
		OneGuideSegments & guide = mCurrent.mSegments[ i ];
		// Get segments count
		guide.mSegments.resize( aInterpolationGroups.getSegmentsCount( pos.mPosition.getUCoordinate(), 
			pos.mPosition.getVCoordinate() ) + 1 );
		// For every segment
		Vector3D< Real > segmentPos;
		Vector3D< Real > segmentSize( 0, 0, aLength / guide.mSegments.size() );
		for ( Segments::iterator segIt = guide.mSegments.begin(); segIt != guide.mSegments.end(); ++segIt )
		{
			*segIt = segmentPos;
			segmentPos += segmentSize; // Next segment position
		}
		// Set segment length
		guide.mSegmentLength = segmentSize.z;
	}
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsDS & aOldRestPositionsDS,
	const GuidesRestPositions & aRestPositions, 
	const Interpolation::InterpolationGroups & aInterpolationGroups,
	unsigned __int32 aNumberOfGuidesToInterpolateFrom ):
	mStartLength( aOldStorage.mStartLength )
{
	if ( aOldStorage.imported() )
	{
		// For every frame
		for ( AllFramesSegments::const_iterator it = aOldStorage.mSegments.begin(); 
			it != aOldStorage.mSegments.end(); ++it )
		{
			InterpolateFrame( it->second, aOldRestPositionsDS, aRestPositions, aInterpolationGroups,
				aNumberOfGuidesToInterpolateFrom,
				// Create output frame
				mSegments.insert( std::make_pair( it->first, FrameSegments() ) ).first->second );
		}
	}
	// Interpolate current segments
	InterpolateFrame( aOldStorage.mCurrent, aOldRestPositionsDS, aRestPositions, aInterpolationGroups, 
		aNumberOfGuidesToInterpolateFrom, mCurrent );
}

SegmentsStorage::SegmentsStorage( const SegmentsStorage & aOldStorage, const GuidesIds & aRemainingGuides ) :
	mStartLength( aOldStorage.mStartLength )
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
			// For every guide
		    #ifdef _OPENMP
            #pragma omp parallel for schedule( guided )
            #endif
			for ( int i = 0; i < static_cast< int >( aRemainingGuides.size() ); ++i )
			{
				// Copy selected guide's segments
				aOutputSegments.mSegments[ i ] = it->second.mSegments[ aRemainingGuides[ i ] ];
			}
		}
	}
	// Import current segments
	mCurrent.mSegments.resize( aRemainingGuides.size() );
	// For every guide
	#ifdef _OPENMP
    #pragma omp parallel for schedule( guided )
    #endif
	for ( int i = 0; i < static_cast< int >( aRemainingGuides.size() ); ++i )
	{
		// Copy selected guide's segments
		mCurrent.mSegments[ i ] = aOldStorage.mCurrent.mSegments[ aRemainingGuides[ i ] ];
	}
}

void SegmentsStorage::setFrame( Time aTime )
{
	if ( !imported() )
	{
		return; // Ignores time
	}
	propagateChangesThroughTime(); // Saves current frame
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
		    #ifdef _OPENMP
            #pragma omp parallel for schedule( guided )
            #endif
			for ( int i = 0; i < static_cast< int >( mCurrent.mSegments.size() ); ++i )
			{
				// Select destination and 2 source guides
				OneGuideSegments & guide = mCurrent.mSegments[ i ];
				Segments::const_iterator upSegIt = upIt[ i ].mSegments.begin();
				Segments::const_iterator lowSegIt = lowIt[ i ].mSegments.begin();
				// For every segment
				for ( Segments::iterator segIt = guide.mSegments.begin(); 
					segIt != guide.mSegments.end(); ++segIt, ++upSegIt, ++lowSegIt )
				{
					// Interpolate
					*segIt = *upSegIt * upFactor + *lowSegIt * lowFactor;
				}
				calculateSegmentLength( guide );
				uniformlyRepositionSegments( guide, static_cast< unsigned __int32 >( guide.mSegments.size() ) );
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

PartialStorage * SegmentsStorage::reinitCuttedHair( Real aLength )
{
	// Prepare partial storage
	PartialStorage * tmpStorage = new PartialStorage;
	// For every guide
	unsigned __int32 id = 0;
	for ( GuidesSegments::iterator it = mCurrent.mSegments.begin(); it != mCurrent.mSegments.end(); ++it, ++id )
	{
		if ( it->mSegmentLength <= EPSILON )
		{
			resetGuideSegments(id, aLength, *it, tmpStorage);
		}
	}
	// Current segments has been changed and need to be propagated through time
	mAreCurrentSegmentsDirty = true;
	return tmpStorage;
}

PartialStorage * SegmentsStorage::resetGuides( Real aLength )
{
	// Prepare partial storage
	PartialStorage * tmpStorage = new PartialStorage;
	// For every guide
	unsigned __int32 id = 0;
	for ( GuidesSegments::iterator it = mCurrent.mSegments.begin(); it != mCurrent.mSegments.end(); ++it, ++id )
	{
		resetGuideSegments(id, aLength, *it, tmpStorage);
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
			// For every guide
		    #ifdef _OPENMP
            #pragma omp parallel for schedule( guided )
            #endif
			for ( int i = 0; i < static_cast< int >( guides.size() ); ++i )
			{
				const GuideRestPosition & pos = aRestPositions[ i ];
				uniformlyRepositionSegments( guides[ i ], 
					// Load new segments count from interpolation groups object
					aInterpolationGroups.getSegmentsCount( pos.mPosition.getUCoordinate(), 
					pos.mPosition.getVCoordinate() ) + 1 );
			}
		}
	}
	// Set segments count for current frame
	GuidesSegments & guides = mCurrent.mSegments;
	// For every guide
	#ifdef _OPENMP
    #pragma omp parallel for schedule( guided )
    #endif
	for ( int i = 0; i < static_cast< int >( guides.size() ); ++i )
	{
		const GuideRestPosition & pos = aRestPositions[ i ];
		uniformlyRepositionSegments( guides[ i ], 
			// Load new segments count from interpolation groups object
			aInterpolationGroups.getSegmentsCount( pos.mPosition.getUCoordinate(), 
			pos.mPosition.getVCoordinate() ) + 1 );
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

void SegmentsStorage::serialize( std::ostream & aOutputStream ) const
{
	Stubble::serialize( mAreCurrentSegmentsDirty, aOutputStream );
	mCurrent.serialize( aOutputStream );
	Stubble::serialize( static_cast< unsigned __int32 >( mSegments.size() ), aOutputStream );
	// over all frames
	for ( AllFramesSegments::const_iterator it = mSegments.begin(); it != mSegments.end(); it++ )
	{
		Stubble::serialize( it->first, aOutputStream );
		it->second.serialize( aOutputStream );		
	}
}

void SegmentsStorage::deserialize( std::istream & aInputStream )
{	
	Stubble::deserialize( mAreCurrentSegmentsDirty, aInputStream );
	mCurrent.deserialize( aInputStream );
	unsigned __int32 frameCount;
	Stubble::deserialize( frameCount, aInputStream );
	mSegments.clear();
	for ( unsigned __int32 i = 0; i < frameCount; ++i )
	{
		// First create frame and then deserialize it ( prevents copy pasting of huge data ! )
		FrameSegments frameSegments;
		Stubble::deserialize( frameSegments.mFrame, aInputStream );
		mSegments.insert( std::make_pair( frameSegments.mFrame, frameSegments ) ). // Store
			first->second.deserialize( aInputStream ); // Deserialize
	}	
}

void SegmentsStorage::calculateSegmentLength( OneGuideSegments & aGuideSegments )
{
	Real length = 0;
	// For every segment
	for ( Segments::const_iterator prevIt = aGuideSegments.mSegments.begin(), segIt = prevIt + 1;
		segIt != aGuideSegments.mSegments.end(); ++segIt, ++prevIt )
	{
		length += Vector3D< Real >( *prevIt, *segIt ).size();
	}
	aGuideSegments.mSegmentLength = length / ( aGuideSegments.mSegments.size() - 1 );
}

void SegmentsStorage::uniformlyRepositionSegments( OneGuideSegments & aGuideSegments, unsigned __int32 aCount )
{
	if ( aCount < 2 )
	{
		throw new StubbleException("New hair segments count less than 2.");
	}

	unsigned __int32 currentSgmtCount = (unsigned __int32)aGuideSegments.mSegments.size();
	Real currentSgmtLength = aGuideSegments.mSegmentLength;
	if ( currentSgmtLength < EPSILON )
	{
		aGuideSegments.mSegments.resize( aCount, Vector3D< Real >() ); // Increase count
		return;
	}
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

	Toolbox::HairTaskProcessor::enforceConstraints(newSegments, newSgmtLegth);

	// Replace data
	aGuideSegments.mSegmentLength = newSgmtLegth;
	aGuideSegments.mSegments = newSegments;
}

void SegmentsStorage::InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsDS & aOldRestPositionsDS,
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
	#ifdef _OPENMP
    #pragma omp parallel for schedule( guided )
    #endif
	for ( int i = 0; i < static_cast< int >( aOutputSegments.mSegments.size() ); ++i )
	{
		const GuideRestPosition & pos = aRestPositions[ i ];
		OneGuideSegments & guide = aOutputSegments.mSegments[ i ];
		// Get interpolation group
		unsigned __int32 interpolationGroup = aInterpolationGroups.getGroupId( pos.mPosition.getUCoordinate(),
																			pos.mPosition.getVCoordinate() );
		// Get segments count
		guide.mSegments.resize( aInterpolationGroups.getGroupSegmentsCount( interpolationGroup ) + 1 ); 
		// Now selected closest guides
		ClosestGuides guidesIds;
		aOldRestPositionsDS.getNClosestGuides( pos.mPosition.getPosition(), 
			interpolationGroup, aNumberOfGuidesToInterpolateFrom, guidesIds );
		if ( guidesIds.size() == 0 ) // No guides in this interpolation group
		{
			// Get segments count
			guide.mSegments.resize( aInterpolationGroups.getSegmentsCount( pos.mPosition.getUCoordinate(), 
				pos.mPosition.getVCoordinate() ) + 1 );
			// For every segment
			Vector3D< Real > segmentPos;
			Vector3D< Real > segmentSize( 0, 0, mStartLength / guide.mSegments.size() );
			for ( Segments::iterator segIt = guide.mSegments.begin(); segIt != guide.mSegments.end(); ++segIt )
			{
				*segIt = segmentPos;
				segmentPos += segmentSize; // Next segment position
			}
			// Set segment length
			guide.mSegmentLength = segmentSize.z;
		}
		else // There are some guides to interpolate from
		{
			// First find max and min
			float maxDistance = sqrtf( guidesIds.begin()->mDistance );
			ClosestGuides::const_iterator first = guidesIds.begin();
			for ( ClosestGuides::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
			{
				// Refresh closest guide
				if ( first->mDistance > guideIdIt->mDistance )
				{
					first = guideIdIt;
				}
			}
			// First check, if they are not on the same position
			if ( first->mDistance < EPSILON || guidesIds.size() == 1 )
			{
				// Just copy
				guide =  aOldSegments.mSegments[ first->mGuideId ];
			}
			else
			{
				// Bias distance with respect to farthest guide
				for ( ClosestGuides::iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
				{
					float & distance = guideIdIt->mDistance;
					distance = sqrtf( distance );
					distance = ( maxDistance - distance ) / ( maxDistance * distance );
					distance *= distance;
				}
				// Finaly calculate cumulated distance
				Real cumulatedDistance = 0;
				for ( ClosestGuides::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
				{
					cumulatedDistance += guideIdIt->mDistance;
				}
				Real inverseCumulatedDistance = 1.0 / cumulatedDistance;
				// For every old guide segments to interpolate from
				for ( ClosestGuides::const_iterator guideIdIt = guidesIds.begin(); guideIdIt != guidesIds.end(); ++guideIdIt )
				{
					// Old segments iterator
					Segments::const_iterator oldSegIt = aOldSegments.mSegments[ guideIdIt->mGuideId ].mSegments.begin();
					Real weight = guideIdIt->mDistance * inverseCumulatedDistance; 
					// For every segment
					for ( Segments::iterator segIt = guide.mSegments.begin(); segIt != guide.mSegments.end(); 
						++segIt, ++oldSegIt )
					{
						*segIt += *oldSegIt * weight;
					}
				}
				calculateSegmentLength( guide );
				uniformlyRepositionSegments( guide, static_cast< unsigned __int32 >( guide.mSegments.size() ) );
			}
		}
	}
}

// May be defined here - is only used inside this cpp file
inline Real SegmentsStorage::timeAffectFactor( Time aTimeDifference, Real aInverseFramesCount )
{
	// Smooth step over all frames
	Real x = aTimeDifference * aInverseFramesCount + 1;
	return x * x * ( 3 - 2 * x );
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
			it != mSegments.end() && ( factor = 
			timeAffectFactor( it->first - mCurrent.mFrame, FRAME_PROPAGATION_TO_FRAMES_INVERSE ) ) > 0; ++it )
		{
			propageteChangesToFrame( it->second.mSegments, factor );
		}
		if ( lowerBound != mSegments.begin() ) // No predecessor
		{
			// Get reverse iterator
			AllFramesSegments::reverse_iterator rIt( lowerBound );
			// For every preceding frame until change has no meaning
			for ( ; rIt != mSegments.rend() && ( factor = 
				timeAffectFactor( mCurrent.mFrame - rIt->first, FRAME_PROPAGATION_TO_FRAMES_INVERSE ) ) > 0; ++rIt )
			{
				propageteChangesToFrame( rIt->second.mSegments, factor );
			}
		}
		
	}
	mAreCurrentSegmentsDirty = false;
}

void SegmentsStorage::propageteChangesToFrame( GuidesSegments & aGuides, Real aFactor )
{
	// For every guide
	#ifdef _OPENMP
    #pragma omp parallel for schedule( guided )
    #endif
	for ( int i = 0; i < static_cast< int >( aGuides.size() ); ++i )
	{
		OneGuideSegments & guide = aGuides[ i ]; // Select guide
		Segments::const_iterator sourceSegIt = mCurrent.mSegments[ i ].mSegments.begin(); // Select source guide
		// For every segment
		for ( Segments::iterator destSegIt = guide.mSegments.begin(); destSegIt != guide.mSegments.end(); 
			++destSegIt, ++sourceSegIt )
		{
			*destSegIt = *sourceSegIt * aFactor + *destSegIt * ( 1 - aFactor );
		}
		calculateSegmentLength( guide );
		uniformlyRepositionSegments( guide, static_cast< unsigned __int32 >( guide.mSegments.size() ) );
	}
}

void SegmentsStorage::resetGuideSegments ( unsigned __int32 aId, Real aLength, OneGuideSegments &aSegments, PartialStorage *aPartialStorage )
{
	// Store old values to tmp storage
	aPartialStorage->mIds.push_back( aId ); // Store guide id
	aPartialStorage->mSegments.push_back( aSegments ); // Store old guide segments
	// For every segment
	Real z = aLength / aSegments.mSegments.size();
	size_t i = 0;
	for ( Segments::iterator segIt = aSegments.mSegments.begin(); segIt != aSegments.mSegments.end(); ++segIt, ++i )
	{
		segIt->set( 0.0, 0.0, i * z );
	}
	// Set segment length
	aSegments.mSegmentLength = z;
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble