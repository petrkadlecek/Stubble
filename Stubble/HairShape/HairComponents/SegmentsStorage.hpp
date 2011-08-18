#ifndef STUBBLE_SEGMENTS_STORAGE_HPP
#define STUBBLE_SEGMENTS_STORAGE_HPP

#include "Primitives\BoundingBox.hpp"
#include "HairShape\HairComponents\GuidePosition.hpp"
#include "HairShape\HairComponents\RestPositionsUG.hpp"
#include "HairShape\HairComponents\Segments.hpp"
#include "HairShape\HairComponents\SelectedGuides.hpp"
#include "HairShape\Interpolation\InterpolationGroups.hpp"

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
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

	///-------------------------------------------------------------------------------------------------
	/// Constructor, interpolates segments from old segments in aOldStorage
	///
	/// \param	aOldStorage				the old segments storage. 
	/// \param	aOldRestPositionsUG		the old rest positions uniform grid. 
	/// \param	aRestPositions			the rest positions of guides. 
	/// \param	aInterpolationGroups	Interpolation groups.
	///-------------------------------------------------------------------------------------------------
	SegmentsStorage( const SegmentsStorage & aOldStorage, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups );

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
	///-------------------------------------------------------------------------------------------------
	void replace( const PartialStorage & aSegmentsChange );

	///-------------------------------------------------------------------------------------------------
	/// Propagate changes of selected guides through all time frames. 
	///
	/// \param	aSelectedGuides	the selected guides. 
	///
	/// \return	segments change in PartialStorage for undo stack 
	///-------------------------------------------------------------------------------------------------
	PartialStorage * propagateChanges( const SelectedGuides & aSelectedGuides );

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

private:

	/// ------------------------------------------------------------------------------------------
	///  Interpolates segments from old segments
	///
	/// \param	aOldSegments			the old segments. 
	/// \param	aOldRestPositionsUG		the old rest positions uniform grid. 
	/// \param	aRestPositions			the rest positions of guides. 
	/// \param	aInterpolationGroups	Interpolation groups. 
	/// \param [in,out]	aOutputSegments	the output segments. 
	///----------------------------------------------------------------------------------------------------
	void InterpolateFrame( const FrameSegments & aOldSegments, const RestPositionsUG & aOldRestPositionsUG,
		const GuidesRestPositions & aRestPositions, 
		const Interpolation::InterpolationGroups & aInterpolationGroups,
		FrameSegments & aOutputSegments ) const;

	AllFramesSegments mSegments;  ///< The all segments (exists after the import!)

	FrameSegments mCurrent; ///< The current frame segments
};

// inline functions implementation

inline SegmentsStorage::~SegmentsStorage()
{
}

inline void SegmentsStorage::importSegments( const FrameSegments & aFrameSegments )
{
	mSegments.insert( std::make_pair( aFrameSegments.mFrame, aFrameSegments ) );
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