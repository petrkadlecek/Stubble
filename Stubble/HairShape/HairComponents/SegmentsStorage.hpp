#ifndef STUBBLE_SEGMENTS_STORAGE_HPP
#define STUBBLE_SEGMENTS_STORAGE_HPP

#include "Common\StubbleException.hpp"
#include "Primitives\BoundingBox.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\RestPositionsDS.hpp"
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
/// Class for storing and operations on all guides segments in all time frames.
/// Enables creation of default segments, interpolation of hair guides from different segments 
/// storage, creating partial copy of segments storage.
/// Works with segments undo stack, propagate changes in one frame to others,
/// allow import of segments for one time frame and other trivial operations.
/// Operates in two modes : before import - only current segments are stored, time changes has no 
/// effects, after import - operates on imported set of time frames
/// This class can be serialized/deserialized. 
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
	/// Segments of each hair guide are interpolated from a requested number of closest guides segments
	/// ( from same interpolation group ) stored in aOldStorage. For closest guides query we use rest 
	/// positions data structure.
	///
	/// \param	aOldStorage							the old segments storage. 
	/// \param	aOldRestPositionsDS					the old rest positions data structure. 
	/// \param	aRestPositions						the rest positions of guides. 
	/// \param	aInterpolationGroups				Interpolation groups.
	/// \param	aNumberOfGuidesToInterpolateFrom	Number of guides to interpolate from
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsDS & aOldRestPositionsDS,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		unsigned __int32 aNumberOfGuidesToInterpolateFrom );

	///-------------------------------------------------------------------------------------------------
	/// Constructor, copies only selected segments from aOldStorage.
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
	/// Propagates changes in current frame to other frames and interpolates new current frame segments
	/// from discretly saved time frames segments.
	///
	/// \param	aTime	Time. 
	///-------------------------------------------------------------------------------------------------
	void setFrame( Time aTime );

	///-------------------------------------------------------------------------------------------------
	/// Replaces the old segments of some guides with aSegmentsChange.
	/// This is used during undo and redo operations. The old version of guides segments which has been 
	/// modified is then returned for undo stack.
	///
	/// \param	aSegmentsChange	The segments change. 
	/// 						
	/// \return	Guides segments before modification for undo stack. 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * replace( const PartialStorage & aSegmentsChange );

	///-------------------------------------------------------------------------------------------------
	/// Propagate changes of selected guides to stored segments.
	/// The old version of guides segments which has been modified is then returned for undo stack.
	///
	/// \param	aSelectedGuides	The selected guides. 
	///
	/// \return	Guides segments before modification for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * propagateChanges( const SelectedGuides & aSelectedGuides );

	///-------------------------------------------------------------------------------------------------
	/// Reinits all cutted hair with zero length to requested initial length.
	/// The old version of guides segments which has been modified is then returned for undo stack.
	///
	/// \param	aLength		Hair guides initial length.
	///
	/// \return	Guides segments before modification for undo stack 
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
	/// Before import is called, segments storage only operates with current frames and changing
	/// time frame has no effect. 
	///
	/// \param	aFrameSegments	the segments in time frame. 
	///-------------------------------------------------------------------------------------------------
	inline void importSegments( const FrameSegments & aFrameSegments );

	///-------------------------------------------------------------------------------------------------
	/// Gets the segments of all hair guides from current time frame. 
	///
	/// \return	The current segments. 
	///-------------------------------------------------------------------------------------------------
	inline const FrameSegments & getCurrentSegments() const;

	///-------------------------------------------------------------------------------------------------
	/// Returns true, if any segments were imported (using importSegments).
	/// Before import is called, segments storage only operates with current frames and changing
	/// time frame has no effect.
	///
	/// \return	true if segments were imported.
	///-------------------------------------------------------------------------------------------------
	inline bool imported() const;

	///-------------------------------------------------------------------------------------------------
	/// Sets the segments count for every guide in all frames.
	/// Can be used when segment count for one interpolation group has changed or all interpolation
	/// groups have changed.
	///
	/// \param	aRestPositions			The rest positions of hair guides' roots. 
	/// \param	aInterpolationGroups	Interpolation groups.
	///-------------------------------------------------------------------------------------------------
	void setSegmentsCount( const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///----------------------------------------------------------------------------------------------------
	/// Gets a bounding box of all segments in current frame. 
	/// The bounding box of all segments is calculated every time this method is called.
	///
	/// \param	aCurrentPositions	the current positions of guides. 
	///
	/// \return	The bounding box. 
	///----------------------------------------------------------------------------------------------------
	BoundingBox getBoundingBox( const GuidesCurrentPositions & aCurrentPositions ) const;
		
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

	///-------------------------------------------------------------------------------------------------
	/// Calculates the segment length as division of total length by segments count 
	///
	/// \param [in,out]	aGuideSegments	The one guide segments. 
	///-------------------------------------------------------------------------------------------------
	static void calculateSegmentLength( OneGuideSegments & aGuideSegments );

	///-------------------------------------------------------------------------------------------------
	/// Uniformly reposition segments with newly set segments count. 
	///
	/// \param [in,out]	aGuideSegments	The guide segments. 
	/// \param aCount					The new number of segments;
	///-------------------------------------------------------------------------------------------------
	static void uniformlyRepositionSegments( OneGuideSegments & aGuideSegments, unsigned __int32 aCount );
	
private:

	/// ------------------------------------------------------------------------------------------
	///  Interpolates segments from old segments ( works only on one time frame ).
	///  Segments of each hair guide are interpolated from a requested number of closest guides segments
	///  ( from same interpolation group ) stored in aOldSegments. For closest guides query we use rest 
	///  positions data structure.
	///
	/// \param	aOldSegments						the old segments. 
	/// \param	aOldRestPositionsDS					the old rest positions data structure. 
	/// \param	aRestPositions						the rest positions of guides. 
	/// \param	aInterpolationGroups				Interpolation groups. 
	/// \param	aNumberOfGuidesToInterpolateFrom	Number of guides to interpolate from
	/// \param [in,out]	aOutputSegments				The interpolated segments. 
	///----------------------------------------------------------------------------------------------------
	void InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsDS & aOldRestPositionsDS,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		unsigned __int32 aNumberOfGuidesToInterpolateFrom, 
		FrameSegments & aOutputSegments ) const;

	///-------------------------------------------------------------------------------------------------
	/// Calculates how much change in one frame affects other frame.
	/// The smooth step scaled to inverse of aInverseFrameCount is used to calculate the factor.
	///
	/// \param	aTimeDifference		Absolute value of distance between frames in time
	/// \param	aInverseFrameCount	Inverse number of frames
	///
	/// \return	Affect factor. 
	///-------------------------------------------------------------------------------------------------
	inline Real timeAffectFactor( Time aTimeDifference, Real aInverseFramesCount );

	///-------------------------------------------------------------------------------------------------
	/// Propagate changes from current frame to other frames.
	/// Must be called before every time change.
	///-------------------------------------------------------------------------------------------------
	void propagateChangesThroughTime();

	///-------------------------------------------------------------------------------------------------
	/// Propagete changes from current frame to selected frame . 
	///
	/// \param [in,out]	aGuides	The guides segments of selected frame. 
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

	AllFramesSegments mSegments;  ///< The all hair guides segments in all frames (exists only after the import!)

	FrameSegments mCurrent; ///< The current frame segments of all hair guides

	bool mAreCurrentSegmentsDirty;  ///< true if the current segments are dirty and need to be propagated to all frames

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