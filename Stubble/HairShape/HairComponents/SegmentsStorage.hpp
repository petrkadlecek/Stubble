#ifndef STUBBLE_SEGMENTS_STORAGE_HPP
#define STUBBLE_SEGMENTS_STORAGE_HPP

#include "Common\StubbleException.hpp"
#include "Primitives\BoundingBox.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\RestPositionsUG.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"
#include "Common\CommonConstants.hpp"
#include "Common\CommonFunctions.hpp"

#include <sstream>
#include <string>

namespace Stubble
{

namespace HairShape
{

namespace HairComponents
{

///-------------------------------------------------------------------------------------------------
/// Class for storing all guides segments .
///-------------------------------------------------------------------------------------------------
class SegmentsStorage
{
public:

	///-------------------------------------------------------------------------------------------------
	/// Constructor, inits segments to point along surface normal. 
	///
	/// \param	aRestPositions			the rest positions of guides. 
	/// \param	aInterpolationGroups	Interpolation groups.
	/// \param	aLength					Hair guides length.
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		Real aLength );

	///-------------------------------------------------------------------------------------------------
	/// Constructor, interpolates segments from old segments in aOldStorage
	///
	/// \param	aOldStorage							the old segments storage. 
	/// \param	aOldRestPositionsUG					the old rest positions uniform grid. 
	/// \param	aRestPositions						the rest positions of guides. 
	/// \param	aInterpolationGroups				Interpolation groups.
	/// \param	aNumberOfGuidesToInterpolateFrom	Number of guides to interpolate from
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		unsigned __int32 aNumberOfGuidesToInterpolateFrom );

	///-------------------------------------------------------------------------------------------------
	/// Constructor, copies only selected segments from aOldStorage
	///
	/// \param	aOldStorage			the old segments storage. 
	/// \param	aRemainingGuides	the remaining guides. 
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const SegmentsStorage & aOldStorage, const GuidesIds & aRemainingGuides );

	///----------------------------------------------------------------------------------------------------
	/// Finalizer
	///----------------------------------------------------------------------------------------------------
	inline ~SegmentsStorage();

	///-------------------------------------------------------------------------------------------------
	/// Sets a time frame. 
	///
	/// \param	aTime	Time. 
	///-------------------------------------------------------------------------------------------------
	void setFrame( Time aTime );

	///-------------------------------------------------------------------------------------------------
	/// Replaces the old segments of some guides with aSegmentsChange (used only for undo, redo) 
	///
	/// \param	aSegmentsChange	the segments change. 
	/// 						
	/// \return	segments change in PartialStorage for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * replace( const PartialStorage & aSegmentsChange );

	///-------------------------------------------------------------------------------------------------
	/// Propagate changes of selected guides through all time frames. 
	///
	/// \param	aSelectedGuides	the selected guides. 
	///
	/// \return	segments change in PartialStorage for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * propagateChanges( const SelectedGuides & aSelectedGuides );

	///-------------------------------------------------------------------------------------------------
	/// Reinits all cutted hair with zero length to selected length.
	///
	/// \param	aLength		Hair guides length.
	///
	/// \return	segments change in PartialStorage for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * reinitCuttedHair( Real aLength );

	///-------------------------------------------------------------------------------------------------
	/// Resets all guides to the selected length - typically default one.
	///
	/// \param	aLength		Hair guides length.
	///
	/// \return	segments change in PartialStorage for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * resetGuides( Real aLength );

	///-------------------------------------------------------------------------------------------------
	/// Import segments in selected time frame. 
	///
	/// \param	aFrameSegments	the segments in time frame. 
	///-------------------------------------------------------------------------------------------------
	inline void importSegments( const FrameSegments & aFrameSegments );

	///-------------------------------------------------------------------------------------------------
	/// Gets the segments from current time frame. 
	///
	/// \return	The current segments. 
	///-------------------------------------------------------------------------------------------------
	inline const FrameSegments & getCurrentSegments() const;

	///-------------------------------------------------------------------------------------------------
	/// Returns true, if any segments were imported (using importSegments)
	///
	/// \return	true if it succeeds, false if it fails. 
	///-------------------------------------------------------------------------------------------------
	inline bool imported() const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the segments count
	///
	/// \param	aRestPositions			the rest positions of guides. 
	/// \param	aInterpolationGroups	Interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void setSegmentsCount( const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///----------------------------------------------------------------------------------------------------
	/// Gets a bounding box. 
	///
	/// \param	aCurrentPositions	the current positions of guides. 
	///
	/// \return	The bounding box. 
		///----------------------------------------------------------------------------------------------------
	BoundingBox getBoundingBox( const GuidesCurrentPositions & aCurrentPositions ) const;

	///-------------------------------------------------------------------------------------------------
	/// Calculates the segment length as division of total length by segments count 
	///
	/// \param [in,out]	aGuideSegments	a guide segments. 
	///-------------------------------------------------------------------------------------------------
	static void calculateSegmentLength( OneGuideSegments & aGuideSegments );

	///-------------------------------------------------------------------------------------------------
	/// Uniformly reposition segments with newly set count. 
	///
	/// \param [in,out]	aGuideSegments	The guide segments. 
	/// \param aCount					The new number of segments;
	///-------------------------------------------------------------------------------------------------
	static void uniformlyRepositionSegments( OneGuideSegments & aGuideSegments, unsigned __int32 aCount );
		
	///-------------------------------------------------------------------------------------------------
	/// Serialize object.
	/// 
	/// \param	aOutputStream	Output stream
	///-------------------------------------------------------------------------------------------------
	void serialize( std::ostream & aOutputStream ) const;

	///-------------------------------------------------------------------------------------------------
	/// Deserialize object.	
	///
	/// \param	aInputStream	Input stream
	///-------------------------------------------------------------------------------------------------
	void deserialize( std::istream & aInputStream );

private:

	/// ------------------------------------------------------------------------------------------
	///  Interpolates segments from old segments
	///
	/// \param	aOldSegments						the old segments. 
	/// \param	aOldRestPositionsUG					the old rest positions uniform grid. 
	/// \param	aRestPositions						the rest positions of guides. 
	/// \param	aInterpolationGroups				Interpolation groups. 
	/// \param	aNumberOfGuidesToInterpolateFrom	Number of guides to interpolate from
	/// \param [in,out]	aOutputSegments				the output segments. 
	///----------------------------------------------------------------------------------------------------
	void InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		unsigned __int32 aNumberOfGuidesToInterpolateFrom, 
		FrameSegments & aOutputSegments ) const;

	///-------------------------------------------------------------------------------------------------
	/// Calculates how much change in one frame affects other frame.
	///
	/// \param	aTimeDifference		Distance between frames in time.
	/// \param	aInverseFrameCount	Inverse number of frames
	///
	/// \return	Affect factor. 
	///-------------------------------------------------------------------------------------------------
	inline Real timeAffectFactor( Time aTimeDifference, Real aInverseFramesCount );

	///-------------------------------------------------------------------------------------------------
	/// Propagate changes from current frame to other frames.
	/// Must be used before segments are exported.
	///-------------------------------------------------------------------------------------------------
	void propagateChangesThroughTime();

	///-------------------------------------------------------------------------------------------------
	/// Propagete changes from current frame to selected frame . 
	///
	/// \param [in,out]	aGuides	The guides of selected frame. 
	/// \param	aFactor			The affect factor ( see timeAffectFactor ). 
	///-------------------------------------------------------------------------------------------------
	void propageteChangesToFrame( GuidesSegments & aGuides, Real aFactor );

	///-------------------------------------------------------------------------------------------------
	/// Resets one guide segments to the supplied length along the local z axis
	///
	/// \param aId Id of the processed guide
	/// \param aLength Desired length of the guide segments
	/// \param[in,out] aSegments Guide segments
	/// \param[in,out] aPartialStorage Partial storage for undo stack purposes
	///-------------------------------------------------------------------------------------------------
	void resetGuideSegments ( unsigned __int32 aId, Real aLength, OneGuideSegments &aSegments, PartialStorage *aPartialStorage );

	AllFramesSegments mSegments;  ///< The all segments (exists after the import!)

	FrameSegments mCurrent; ///< The current frame segments

	bool mAreCurrentSegmentsDirty;  ///< true if are current segments dirty and need to be propagated

	Real mStartLength;  ///< The start length of hair guides
};

// inline functions implementation

inline SegmentsStorage::~SegmentsStorage()
{
}

inline void SegmentsStorage::importSegments( const FrameSegments & aFrameSegments )
{
	mSegments.insert( std::make_pair( aFrameSegments.mFrame, aFrameSegments ) );
	mAreCurrentSegmentsDirty = false; // Signal that current frame is useless
}

inline const FrameSegments & SegmentsStorage::getCurrentSegments() const
{
	return mCurrent;
}

inline bool SegmentsStorage::imported() const
{
	return !mSegments.empty();
}

} // namespace HairComponents

} // namespace HairShape

} // namespace Stubble

#endif // STUBBLE_SEGMENTS_STORAGE_HPP